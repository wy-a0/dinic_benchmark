import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def slug_type(s: str) -> str:
    s = s.strip().lower()
    s = (s.replace("ü", "ue")
           .replace("ö", "oe")
           .replace("ä", "ae")
           .replace("ß", "ss"))
    s = s.replace("-", "").replace(" ", "")
    return s

def maybe_log_scale(ax, y_values):
    y = np.array([v for v in y_values if v is not None and np.isfinite(v) and v > 0])
    if len(y) < 2:
        return
    ratio = y.max() / y.min()
    if ratio >= 200:  
        ax.set_yscale("log")

def main():
    csv_path = sys.argv[1] if len(sys.argv) > 1 else "results.csv"

    df = pd.read_csv(csv_path)

    required = {"V", "E", "Typ", "Instance", "EK_ms", "Dinic_ms"}
    missing = required - set(df.columns)
    if missing:
        raise SystemExit(f"CSV fehlt Spalten: {missing}\nGefunden: {list(df.columns)}")

    # Typen in sinnvoller Reihenfolge
    type_order = ["Dünn", "Dicht", "Unit-Cap"]
    types = [t for t in type_order if (df["Typ"] == t).any()]
    for t in sorted(df["Typ"].unique()):
        if t not in types:
            types.append(t)

    for t in types:
        sub = df[df["Typ"] == t].copy()
        if sub.empty:
            continue

        # Mittelwert / Std pro V über Instances
        g = (
            sub.groupby("V", as_index=False)
               .agg(
                    EK_mean=("EK_ms", "mean"),
                    EK_std=("EK_ms", "std"),
                    Dinic_mean=("Dinic_ms", "mean"),
                    Dinic_std=("Dinic_ms", "std"),
                    n=("Instance", "nunique"),
                )
               .sort_values("V")
        )
        g["EK_std"] = g["EK_std"].fillna(0.0)
        g["Dinic_std"] = g["Dinic_std"].fillna(0.0)

        tag = slug_type(t)

       
        # 1) Balkendiagramm (pro V)
       
        fig, ax = plt.subplots(figsize=(11, 5.5))
        x = np.arange(len(g))
        width = 0.42

       
        ax.bar(x - width/2, g["EK_mean"], width, label="Edmonds–Karp",
               edgecolor="none", linewidth=0)
        ax.bar(x + width/2, g["Dinic_mean"], width, label="Dinic",
               edgecolor="none", linewidth=0)

        ax.set_xticks(x)
        ax.set_xticklabels(g["V"].astype(int))
        ax.set_xlabel("V (Anzahl Knoten)")
        ax.set_ylabel("Zeit (ms)")
        ax.set_title(f"{t}: Mittelwert pro V (jeweils über {int(g['n'].max())} Instanzen)")
        ax.grid(axis="y", alpha=0.3)
        ax.legend()

        fig.tight_layout()
        fig.savefig(f"bar_{tag}.png", dpi=300, bbox_inches="tight")
        plt.close(fig)

       
        # 2) Linienplot (Mean ± Std)
     
        fig, ax = plt.subplots(figsize=(11, 5.5))

        ax.plot(g["V"], g["EK_mean"], marker="o", linewidth=2, label="Edmonds–Karp")
        ax.plot(g["V"], g["Dinic_mean"], marker="o", linewidth=2, label="Dinic")

        # schattiertes Band ±Std , deutlichere Streuung
        ek_lo = (g["EK_mean"] - g["EK_std"]).clip(lower=0)
        ek_hi = (g["EK_mean"] + g["EK_std"])
        di_lo = (g["Dinic_mean"] - g["Dinic_std"]).clip(lower=0)
        di_hi = (g["Dinic_mean"] + g["Dinic_std"])

        ax.fill_between(g["V"], ek_lo, ek_hi, alpha=0.18)
        ax.fill_between(g["V"], di_lo, di_hi, alpha=0.18)

        ax.set_xlabel("V (Anzahl Knoten)")
        ax.set_ylabel("Zeit (ms)")
        ax.set_title(f"{t}: Laufzeit über V (Mittelwert ± Std)")
        ax.grid(alpha=0.3)
        ax.legend()

      
        maybe_log_scale(ax, list(g["EK_mean"]) + list(g["Dinic_mean"]))

        fig.tight_layout()
        fig.savefig(f"line_{tag}.png", dpi=300, bbox_inches="tight")
        plt.close(fig)

     
        # 3) Boxplot (ohne Ausreißer-Kreise)
    
        fig, ax = plt.subplots(figsize=(7.5, 5.5))

        data = [sub["EK_ms"].values, sub["Dinic_ms"].values]

        ax.boxplot(
            data,
            labels=["Edmonds–Karp", "Dinic"],
            showfliers=False,     #  Kreise weg
            whis=(5, 95),         # robuste Whisker
        )

        ax.set_ylabel("Zeit (ms)")
        ax.set_title(f"{t}: Verteilung über alle Instanzen")
        ax.grid(axis="y", alpha=0.3)

     
        maybe_log_scale(ax, list(sub["EK_ms"]) + list(sub["Dinic_ms"]))

        fig.tight_layout()
        fig.savefig(f"box_{tag}.png", dpi=300, bbox_inches="tight")
        plt.close(fig)

    print("Fertig. Erzeugt wurden:")
    for t in types:
        tag = slug_type(t)
        print(f"  bar_{tag}.png, line_{tag}.png, box_{tag}.png")

if __name__ == "__main__":
    main()

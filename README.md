# Dinic vs. Edmonds–Karp Benchmark (Max-Flow)

Dieses Projekt benchmarked und vergleicht zwei Max-Flow-Algorithmen (**Dinic** und **Edmonds–Karp**) auf verschiedenen Graph-Instanzen.  
Die Benchmark-Ergebnisse werden als `results.csv` gespeichert und anschließend automatisch als **Plots** visualisiert.

## Inhalte / Features
- Implementierung von **Dinic** und **Edmonds–Karp** (C++, C++17)
- Benchmark-Ausführung über verschiedene Instanzen/Graph-Typen
- Export der Laufzeiten nach `results.csv`
- Automatische Plot-Erzeugung (PNG):
  - Balkendiagramm (Mittelwerte)
  - Linienplot (Mittelwert ± Std)
  - Boxplot (Verteilungen)

## Voraussetzungen
### Build
- `g++` (C++17)
- `make`

### Plots
- Python (empfohlen: conda/env), mit:
  - `numpy`
  - `pandas`
  - `matplotlib`

> Hinweis (macOS/conda):  
> Wenn `python3` bei dir auf das System-Python zeigt (z.B. `/Library/Frameworks/...`), dann laufen die Plots oft **ohne** die conda-Pakete.  
> Nutze in dem Fall stattdessen die conda-Version: `python src/plot_results.py results.csv`.  
> Prüfen kannst du das so: `which python` und `which python3`.


## Installation (Python Libraries)


### Python-Abhängigkeiten installieren (pip)
python3 -m pip install numpy pandas matplotlib

## Projekt ausführen

### 1) Alles automatisch (Build + Benchmark + Plots)
make

Erzeugt:
- Binary: max_flow_benchmark
- Ergebnisdatei: results.csv
- Plots: plots/ (PNG-Dateien)

### 2) Nur Benchmark ausführen
make run

### 3) Nur Plots erzeugen (aus vorhandener results.csv)
make plot

### 4) Aufräumen
make clean

## Output-Dateien

### results.csv
Enthält pro Instanz u.a.:
- V, E
- Typ
- Instance
- EK_ms (Edmonds–Karp Laufzeit in ms)
- Dinic_ms (Dinic Laufzeit in ms)

### plots
Enthält pro Typ (z.B. Dünn/Dicht/Unit-Cap) jeweils:
- bar_<typ>.png
- line_<typ>.png
- box_<typ>.png


## Troubleshooting

### Plots werden nicht erzeugt
Prüfe, welches Python genutzt wird:
which python
which python3

Teste, ob die Libraries installiert sind:
python -c "import numpy, pandas, matplotlib; print('ok')"

Erzeuge Plots manuell:
python src/plot_results.py results.csv

### Fehler: ModuleNotFoundError: No module named 'numpy'
Dann läuft dein Script mit einem Python, in dem numpy nicht installiert ist.
Nutze conda-python (python) oder installiere die Pakete in genau dieses Python.

## Autor
Wesam Al-Hayani
TU Dortmund

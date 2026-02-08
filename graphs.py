import argparse
import os
import pandas as pd
import matplotlib.pyplot as plt

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--points", default="points.csv")
    parser.add_argument("--stats", default="stats.csv")
    parser.add_argument("--run", type=int, default=0)
    parser.add_argument("--x", choices=["seen", "step"], default="seen")
    parser.add_argument("--outdir", default="")
    parser.add_argument("--show", action="store_true")
    args = parser.parse_args()

    points = pd.read_csv(args.points)
    stats = pd.read_csv(args.stats)

    points_run = points[points["run"] == args.run].copy()
    if points_run.empty:
        raise SystemExit(f"Нет строк для run={args.run} в {args.points}")

    xcol = args.x
    points_run = points_run.sort_values(xcol)
    stats = stats.sort_values(xcol)

    if xcol == "seen":
        xlabel = "Размер обработанной части потока"
    else:
        xlabel = "Номер шага (момент времени t)"

    fig1 = plt.figure()
    plt.plot(points_run[xcol], points_run["F0"], label="F0 (точное)")
    plt.plot(points_run[xcol], points_run["Nt"], label="Nt (оценка HLL)")
    plt.xlabel(xlabel)
    plt.ylabel("Число уникальных элементов")
    plt.title(f"График 1: сравнение Nt и F0 (поток {args.run})")
    plt.legend()
    plt.tight_layout()

    fig2 = plt.figure()
    x = stats[xcol]
    mean = stats["meanNt"]
    std = stats["stdNt"]
    lower = mean - std
    upper = mean + std

    plt.plot(x, mean, label="E(Nt)")
    plt.fill_between(x, lower, upper, alpha=0.5, label="Область неопределенности: E(Nt) ± σ")
    plt.plot(x, lower, linewidth=1, label="Нижняя граница: E(Nt) - σ")
    plt.plot(x, upper, linewidth=1, label="Верхняя граница: E(Nt) + σ")

    plt.xlabel(xlabel)
    plt.ylabel("Число уникальных элементов")
    plt.title("График 2: статистики оценки (E(Nt) и область ±σ)")
    plt.legend()
    plt.tight_layout()

    if args.outdir:
        os.makedirs(args.outdir, exist_ok=True)
        fig1.savefig(os.path.join(args.outdir, "graph1.png"), dpi=200, bbox_inches="tight")
        fig2.savefig(os.path.join(args.outdir, "graph2.png"), dpi=200, bbox_inches="tight")

    if args.show or not args.outdir:
        plt.show()

if __name__ == "__main__":
    main()

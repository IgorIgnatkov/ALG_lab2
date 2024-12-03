import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

# Функция для логарифмической регрессии
def log_func(x, a, b):
    return a * np.log(x) + b

# Чтение данных из файла и построение графика для AVL-дерева
def process_tree_data(file_name, tree_type):
    n_values = []
    heights = []

    with open(file_name, 'r') as file:
        for line in file:
            parts = line.split(',')
            n = int(parts[0].split('=')[1].strip())
            height = int(parts[1].split('=')[1].strip())
            n_values.append(n)
            heights.append(height)

    n_values = np.array(n_values)
    heights = np.array(heights)

    # Логарифмическая регрессия
    params, _ = curve_fit(log_func, n_values, heights)
    a, b = params
    print(f"{tree_type} логарифмическая регрессия: y = {a:.4f} * ln(x) + {b:.4f}")

    # Построение графика
    plt.figure(figsize=(12, 6))
    plt.plot(n_values, heights, label=f"{tree_type} экспериментальные данные", marker='o', linestyle='')
    plt.plot(n_values, log_func(n_values, a, b), label=f"{tree_type} регрессия", linestyle='--')
    plt.xlabel("Количество ключей")
    plt.ylabel("Высота дерева")
    plt.legend()
    plt.grid()
    plt.title(f"Зависимость высоты {tree_type} дерева от количества ключей")
    plt.savefig(f"{tree_type}_height.png")
    plt.close()

    return n_values, heights, log_func(n_values, a, b)

# Чтение данных и построение графиков для каждого типа дерева
avl_n_values, avl_heights, avl_regression = process_tree_data('tree_heights_AVL.txt', 'AVL')
rb_n_values, rb_heights, rb_regression = process_tree_data('tree_heights_RB.txt', 'RB')
bst_n_values, bst_heights, bst_regression = process_tree_data('tree_heights_BST.txt', 'BST')

# Совместный график для всех трех типов деревьев
plt.figure(figsize=(12, 6))
plt.plot(avl_n_values, avl_heights, label="AVL экспериментальные данные", marker='o', linestyle='')
plt.plot(avl_n_values, avl_regression, label="AVL регрессия", linestyle='--')
plt.plot(rb_n_values, rb_heights, label="RB экспериментальные данные", marker='s', linestyle='')
plt.plot(rb_n_values, rb_regression, label="RB регрессия", linestyle='--')
plt.plot(bst_n_values, bst_heights, label="BST экспериментальные данные", marker='^', linestyle='')
plt.plot(bst_n_values, bst_regression, label="BST регрессия", linestyle='--')
plt.xlabel("Количество ключей")
plt.ylabel("Высота дерева")
plt.legend()
plt.grid()
plt.title("Зависимость высоты дерева от количества ключей для всех типов деревьев")
plt.savefig("all_trees_height.png")
plt.close()

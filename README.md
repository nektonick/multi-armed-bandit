# Multi-armed bandit
## Applying reinforcement learning to the "multi-armed bandit" problem
### Research steps
1. Build a class for a multi-armed bandit model with `N` arms
1. Create arm selection function according to the available probability distribution in the problem
1. Create a random arm selection function
1. Add reward for the selected turn
1. Add a function for modifying the probability distribution after the reward for the turn has become known
1. For each turn, enter the cost of a certain amount of resources available to the player

### The purpose of research
1. Demonstrate the change in the probability distribution of choosing the i-th arm with increasing turn number
1. Demonstrate the dependence of the probability of choosing the "best" arm with increasing turn number
1. Demonstrate the change in the agent's payoff over time


# Многорукий бандит
## Применение обучения с подкреплением в задаче многорукого бандита
### План работы
1. Построить класс для модели многорукого бандита с `N` ручками
1. Ввести функцию выбора ручки согласно имеющемуся распределению вероятностей в задаче
1. Ввести функцию случайного выбора ручки
1. Ввести вознаграждение за выбранный ход
1. Ввести функцию модификации распределения вероятностей после того, как стало известно вознаграждение за ход
1. Для каждого хода ввести стоимость некоторого количества ресурсов, имеющихся у игрока

### Цель работы
1. Продемонстрировать изменение распределения вероятностей выбора i-й ручки с ростом номера хода
1. Продемонстрировать зависимость вероятности  выбора  "лучшей"  ручки  с  ростом номера хода
1. Продемонстрировать изменение выигрыша агента со временем

### Использованные материалы
- [Формулировка задачи](https://star-wiki.ru/wiki/Multi-armed_bandit)
- [Многорукие бандиты: введение и алгоритм UCB1](https://habr.com/ru/company/surfingbird/blog/168611/)
- [Машинное обучение. Обучение с подкреплением. К.В. Воронцов](https://www.youtube.com/watch?v=iEUrX_eEWNY)
- [Предварительно скомпилированные заголовки (stdafx.h)](https://habr.com/ru/company/pvs-studio/blog/227521/)
- [Введение в CMake](https://neerc.ifmo.ru/wiki/index.php?title=CMake_Tutorial)
- [Флаги компилятора (warnings)](https://habr.com/ru/post/490850/)
- [clang-format](https://clang.llvm.org/docs/ClangFormatStyleOptions.html)
- [Конструкторы дочерних классов](https://www.learncpp.com/cpp-tutorial/constructors-and-initialization-of-derived-classes/)

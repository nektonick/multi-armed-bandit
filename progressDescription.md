## *Тут даны более подробные комментарии по структуре классов и их методам*

---

Во-первых, будет класс "руки" - представление одного из рычагов автомата. Поскольку у "руки" может быть несколько реализаций, создадим базовый интерфейс:
```C++
class IArm
{
public:
    IArm();
    // У базового класса должен быть виртуальный деструктор
    virtual ~IArm() = default;
    virtual double pull() = 0;
    virtual double getRewardExpectation() = 0;
    virtual std::string toString() = 0;
protected:
    std::mt19937 randomEngine;
};
```
Все его методы чисто-виртуальные и должны быть реализованы наследниками. При этом всех наследников можно будет хранить по указателю на базовый класс и работать с ними, не вдаваясь в детали их реализации.
Так как при нажатии руки выигрыш выдаётся с некоторой вероятностью, в классе также будет генератор случайных чисел (`randomEngine`). Он инициализируется в конструкторе базового класса. Важно, чтобы он инициализировался действительно случайным числом, а не псевдо-случайным. Вариант с инициализацией от текущего времени (`time(0)`) тут не подходит - в таком случае если создавать много рук подряд у них у всех будут одинаковые вероятности. Так что для генерации зерна randomEngine используется более дорогостоящий в плане производительности `std::random_device`.

Метод `pull()` эмулирует нажатие рычага и возвращает выигрыш (или 0, если ничего не выиграно).
Метод `getRewardExpectation()` возвращает аналог вероятности выигрыша (критерий, по которому можно понять, какая из рук в итоге будет прибыльнее). Для реализаций рук, которые всегда возвращают выигрыш (то есть, отличается только его минимальное/максимальное значение или распределение) в качестве такого показателя будет выступать средневзвешенное значение распределения выигрышей.
Метод `toString()` возвращает информацию о руке в текстовом виде (нужен для логирования).

В работе есть две реализации интерфейса:
`BernoulliArm` возвращает 0 в случае проигрыша и 1 (умноженную на какой-нибудь коэффициент) в случае выигрыша. Вероятность выигрыша `rewardChance_` определяется дробным числом от 0 до 1 и задаётся в конструкторе объекта.
`NormalArm` возвращает случайное число из промежутка `[rewardMin_, rewardMax_]`.

---

Во-вторых, будет класс самого "многорукого бандита". В нём будет храниться массив рук (не обязательно одного типа), стоимость одного нажатия. В нём также можно хранить "служебную" информацию, например - о том, какая рука самая лучшая (прибыльная).
Базовый класс снова будет представлен интерфейсом с чисто абстрактными методами: 
```C++
class IBandit
{
public:
    IBandit();
    virtual ~IBandit() = default;
    virtual double pullArmAndGetReward(size_t armIndex) = 0;
    virtual double getPullCost() const = 0;
    virtual size_t getArmsCount() const = 0;
    virtual std::string statePrint() = 0;
};
```
Сами методы будет определены в наследнике: 
```C++
// Чтобы вручную не следить за памятью, используем умный указатель std::shared_ptr. Но так как писать это каждый раз не хочется, воспользуемся "элиасом":
using ArmPtr = std::shared_ptr<IArm>;

// Наследник
class SimpleBandit : public IBandit
{
public:
    // Сразу ждём, что нам передадут информацию о наших "руках" при создании и цену нажатия руки (в этой реализации она всегда одинаковая для любой руки и по умолчанию равна нулю (бесплатная)) 
    SimpleBandit(const std::vector<ArmPtr>& arms, double pullCost_ = 0);
    // Расскажем про наши руки (в основном для логов)
    std::string statePrint() override;

    // Эмулируем нажатие определённой руки. Возвращаем выигрыш. (Здесь ещё можно было бы списывать деньги у нажавшего, передавая double& cash и вычитая из него, но в данной реализации вычитание денег оставлено за пользователем данного класса)
    double pullArmAndGetReward(size_t armIndex) override;
    // Геттер для цены нажатия
    double getPullCost() const override;
    // Сколько всего у нас рук
    virtual size_t getArmsCount() const override;

protected:
    std::vector<ArmPtr> arms_;
    size_t bestArmIndex_;
    const double pullCost_;
};
```

---

В третьих, нужны функции/алгоритмы выбора руки для игрока(агента). Так как алгоритмы довольно сложные и им нужны свои данные, то они тоже будут представлены классом. 
Базовый интерфейс (как всегда, с чисто виртуальными функциями):
```C++
class IStrategy
{
public:
    // Данных нет, так что конструктор и деструктор будут по-умолчанию. Их можно было вообще не определять, если бы деструктор не должен был бы быть виртуальным.
    IStrategy() = default;
    virtual ~IStrategy() = default;
    // Возвращает индекс руки, которую лучше всего нажать сейчас
    virtual size_t selectNextArm() = 0;
    // Обновляет данные после нажатия руки (какую нажали, сколько выиграли). Именно тут должно происходить "обучение".
    virtual void updateState(size_t selectedArmIndex, double reward) = 0;
    // Вернёт название стратегии (для логов)
    virtual std::string getName() = 0;
    // Возвращает внутренне представление того, насколько выгодно нажать каждую из рук. Чем выше значение, тем более выгодной стратегия считает данную руку.
    virtual std::vector<double> getArmsExpectation() = 0;
};
```

У этого интерфейса может быть очень много разных наследников. В большинстве из них используются сложные математические алгоритмы, реализация которых была взята из статей и аналогичных проектов. К сожалению, названия переменных в них не очень понятны, поскольку алгоритмы писались математиками (или плохими программистами).

Рассмотрим "случайную" стратегию. Именно с ней мы в дальнейшем будем сравнивать результаты остальных наших стратегий. Если какая-та стратегия показывает результаты хуже, чем случайная, то она считается неэффективной.
```C++
class RandomStrategy : public IStrategy
{
public:
    // Важно знать, сколько всего рук у автомата
    RandomStrategy(size_t armsCount);
    // Вернёт случайную руку (индекс в интервале [0, armsCount-1])
    size_t selectNextArm() override;
    // Случайная стратегия не обучается, так что тут мы ничего не делаем
    void updateState(size_t selectedArmIndex, double reward) override;
    std::string getName() override;
    // Ожидания от всех рук для случайной стратегии будут равны
    std::vector<double> getArmsExpectation() override;
    // Если вдруг захочется вручную задать зерно для генератора случайных чисел после создания экземпляра класса
    void setRandomSeed(uint8_t randomSeed);

protected:
    std::mt19937 randomEngine;
    const size_t armsCount_;
};
```

---

В четвёртых, создадим класс для игрока(агента). В сложных вариантах он может содержать в себе несколько стратегий и менять их в зависимости от внешних факторов, но для упрощения в базовом интерфейсе будет только одна используемая стратегия:
```C++
// Снова используем "элиас"
using StrategyPtr = std::shared_ptr<IStrategy>;

class IAgent
{
public:
    // При создании нам передают какую-то конкретную стратегию по указателю на базовый класс и число денег/ресурсов.
    IAgent(StrategyPtr strategy, double startCash);
    virtual ~IAgent() = default;

    // А этот метод можно определить уже в базовом классе:
    // Получаем у стратегии руку, которую нужно нажать, нажимаем (для этого нужен входной параметр `bandit`), получаем выигрыш, передаём данные обратно в стратегию. Наследники могут усложнить данный алгоритм.
    virtual void runSingleRound(std::shared_ptr<IBandit> bandit);
    // Вызывает соответствующий метод у хранимой стратегии
    virtual std::string printStrategyName() const = 0;
    // Геттер для денег
    virtual double getCash() const;
    // Вызывает соответствующий метод у хранимой стратегии
    virtual std::vector<double> getArmsExpectation() const = 0;

protected:
    StrategyPtr strategy_;
    double cash_;
};
``` 

---

В пятых, так как есть необходимость следить за изменением распределения вероятностей и демонстрировать изменения выигрыша со временем, то введём класс-логгер, который будет хранить эти изменения и показывать:
```C++
// Большая часть данных будет привязана к раунду симуляции
class Logger
{
public:
    Logger();
    virtual ~Logger() = default;
    // Сохраняет реальные данные рук бандита
    virtual void logActualArmsStats(const std::vector<ArmPtr>& arms);
    // Сохраняет данные по агенту - количество денег, представления о выигрыше рук
    virtual void logActualAgentState(std::shared_ptr<IAgent> agent);
    // Говорим, что начался следующий раунд
    void increaceIteration();
    // Возвращает все накопленные за прошедшие раунды данные
    virtual std::string printAll();
    // Возвращает накопленные данные только о числе денег агентов (в каждом раунде) 
    virtual std::string printCacheStats();
protected:
    // Структура для внутреннего представления данных (состояние агента при очередной итерации)
    struct AgentStatsPerIteration
    {
        // Имя агента (или его стратегии)
        std::string name;
        // его деньги
        double cache = 0;
        // его представление о руках
        std::vector<double> armsCoeffs;
    };
    // Так как у нас будет много агентов, то будем использовать массив. Создадим "элиас" для удобства
    using agentsVector = std::vector<AgentStatsPerIteration>;

    // текущая итерация
    size_t iteration = 0;
    // Настоящие данные о "выигрышности" рук
    std::vector<double> actualArmsStats;
    // Состояние агентов по итерациям (нулевой элемент массива - состояние агентов в нулевую итерацию и т.д.)
    std::vector<agentsVector> agentStatsPerIteration;
};
```

---

Теперь можно используя созданные классы провести симуляцию:
```C++
int main()
{
    // С какими параметрами будем запускаться:

    // Число рук бандита
    const size_t armsCount = 3;
    // Число итераций симуляции
    const size_t iterationsCount = 10000;
    // Стоимость нажатия руки
    const double pullCost = 0.5;
    // Множитель выигрыша
    const double rewardMultiplier = 1;

    // Логгер и перечисление, отражающее его настройку - как много информации выводить 
    // Для большого числа итерацией рекомендуется `none` или `cacheOnly`
    Logger logger;
    enum class LogOptions
    {
        none = 0,
        cacheOnly,
        full
    };
    // Задали уровень логов (как много выводить)
    LogOptions logOptinos = LogOptions::cacheOnly;

    // Создаём руки. В данном случае все руки будут типа BernoulliArm
    std::vector<ArmPtr> arms;
    for(size_t i = 0; i < armsCount; ++i)
    {
        auto arm = std::make_shared<BernoulliArm>(rewardMultiplier);
        arms.push_back(arm);
    }

    // Создаём бандита и передаём ему массив рук
    std::shared_ptr<SimpleBandit> bandit = std::make_shared<SimpleBandit>(arms, pullCost);

    // Мы хотим проверить разные стратегии, так что агентов у нас будет много (хотя бандит и один. Можно было бы под каждого игрока создавать свой автомат, но тогда автоматы будут разные и сравнение окажется некорректным)
    std::vector<std::shared_ptr<SimpleAgent>> agents =
    // тут создаётся массив игроков. Каждому игроку в конструктор подаётся стратегия, а ей при создании на вход подаётся число рук бандита
    {
        // нулевая стратегия
        std::make_shared<SimpleAgent>(std::make_shared<RandomStrategy>(bandit->getArmsCount())),
        // первая стратегия
        std::make_shared<SimpleAgent>(std::make_shared<DMED_BinaryStrategy>(bandit->getArmsCount())),
        // вторая стратегия
        std::make_shared<SimpleAgent>(std::make_shared<KL_UCBStrategy>(bandit->getArmsCount()))
    };

    // Перед стартом теста запомнили информацию о руках бандита (хотя можно и после теста запомнить. Сами руки не меняются)
    logger.logActualArmsStats(arms);

    std::cout << "Start of test" ENDL;
    // чтобы пользователь не думал, что мы зависли, будем печатать прогресс-бар
    std::cout << '[';
    std::flush(std::cout);

    // Начали крутиться по итерациям
    for(size_t iteration = 0; iteration < iterationsCount; ++iteration)
    {
        // Каждый агент делает одно и то же (отличается только стратегия выбора руки)
        for(auto& agent : agents)
        {
            agent->runSingleRound(bandit);
            // запомнили его выбор и изменение сосояния
            logger.logActualAgentState(agent);
        }
        // С агентами всё, можно сказать логгеру, что раунд кончился
        logger.increaceIteration();

        // Логика, отвечающая за печать прогресс-бара
        if(iterationsCount >= 100)
        {
            if(iteration % (iterationsCount / 100) == 0)
            {
                std::cout << '=';
            }
        }
        else
        {
            /// string of (100 / iterationsCount) symbols
            std::string loadingStringPart(100 / iterationsCount, '=');
            std::cout << loadingStringPart;
        }
        // force to output the buffer
        std::flush(std::cout);
    }
    std::cout << ']';
    std::cout << ENDL "End of test" ENDL;

    // После окончания теста выводим информацию, если требуется
    switch(logOptinos)
    {
    case LogOptions::cacheOnly:
        std::cout << ENDL << logger.printCacheStats();
        break;
    case LogOptions::full:
        std::cout << ENDL << logger.printAll();
        break;
    case LogOptions::none:
        // nothing (drop down to default)
    default:
        std::cout << "Output minimized";
        break;
    }

    // Ошибок не случилось, возвращаем 0.
    return 0;
}
```
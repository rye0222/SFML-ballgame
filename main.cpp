#include<SFML/Graphics.hpp>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<string> // 新增
using namespace std;
using namespace sf;
//随机小球生成
void generateRandomBall(vector<CircleShape>& balls, float maxX, float maxY)
{
    float radius = static_cast<float>(rand() % 15 + 5); // 随机半径
    float x =static_cast<float>(rand())/static_cast<float>(RAND_MAX); // 圆心X
    float y = static_cast<float>(rand())/static_cast<float>(RAND_MAX); // 圆心Y
    float centerX = radius+x*(maxX-2*radius); // 确保圆心在边界内
    float centerY = radius + y * (maxY - 2 * radius); // 确保圆心在边界内	
    CircleShape ball(radius);
    ball.setFillColor(Color(rand() % 256, rand() % 256, rand() % 256)); // 随机颜色
    ball.setOrigin(radius, radius); // 设置原点为圆心
    ball.setPosition(centerX, centerY);         // 圆心坐标
    balls.emplace_back(ball);
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    VideoMode desktop = VideoMode::getDesktopMode();
    RenderWindow window(desktop, "", Style::Fullscreen);
    window.setFramerateLimit(60);
    const float speed = 200.f;
    bool isAlive = true; // AI小球是否存活
    class Ball
    {
    public:
        float x, y;         // 定义小球
        float radius;
        Color color;
        CircleShape shape;

        // Ball 类的构造函数
        Ball(float x, float y, float radius, Color color)
            : x(x), y(y), radius(radius), color(color)
        {
            shape.setRadius(radius);
            shape.setFillColor(color);
            shape.setPosition(x, y);
        }
    };

    vector<Ball>balls;   //玩家小球
    vector<CircleShape> smallballs;   //随机小球
    balls.emplace_back(100.f, 200.f, 50.f, Color::Red);  // 小球1
    balls.emplace_back(500.f, 300.f, 50.f, Color::Blue); // 小球2

    // ----------- 新增：得分系统相关变量 -----------
    int redScore = 0;
    int blueScore = 0;
    Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) // 路径可根据实际情况调整
        return -1;
    Text redText, blueText;
    redText.setFont(font);
    redText.setCharacterSize(36);
    redText.setFillColor(Color::Red);
    redText.setPosition(20.f, 20.f);

    blueText.setFont(font);
    blueText.setCharacterSize(36);
    blueText.setFillColor(Color::Blue);
    blueText.setPosition(static_cast<float>(desktop.width) - 200.f, 20.f);
    // -------- 新增：Game Over 文本初始化 --------
    Text gameOverText, winnerText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(96);
    gameOverText.setFillColor(Color::White);
    gameOverText.setStyle(Text::Bold);

    winnerText.setFont(font);
    winnerText.setCharacterSize(48);
    winnerText.setFillColor(Color::Yellow);
    winnerText.setStyle(Text::Bold);
    // ----------------------------------------
    //*****************AI小球*********************

    class AIBall
    {
    public:
        float x, y;
        float radius;
        Color color;
        CircleShape shape;

        // 正确的构造函数
        AIBall(float x, float y, float radius, Color color)
            : x(x), y(y), radius(radius), color(color)
        {
            shape.setRadius(radius);
            shape.setFillColor(color);
            shape.setOrigin(radius, radius);
            shape.setPosition(x, y);
        }
    };
    // 初始化AI小球，半径20，绿色
    AIBall aiBall(300.f, 400.f, 20.f, Color::Green);

    Clock clock; // 用于计算时间差
    const float spawnInterval = 0.5f; // 随机小球生成间隔
    const int ballcount = 200;
    for (int i = 0;i < ballcount;++i)
    {
        generateRandomBall(smallballs, static_cast<float>(desktop.width), static_cast<float>(desktop.height));
    }
    while (window.isOpen())
    {
        float deltatime = clock.restart().asSeconds(); // 计算时间差
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed || e.key.code == Keyboard::Escape)  // 处理玩家输入
                window.close();
        }

        // 控制小球1移动
        if (Keyboard::isKeyPressed(Keyboard::W))
        {
            balls[0].y -= speed * deltatime; // 小球1上移
        }
        if (Keyboard::isKeyPressed(Keyboard::A))
        {
            balls[0].x -= speed * deltatime; // 小球1左移
        }
        if (Keyboard::isKeyPressed(Keyboard::S))
        {
            balls[0].y += speed * deltatime; // 小球1下移
        }
        if (Keyboard::isKeyPressed(Keyboard::D))
        {
            balls[0].x += speed * deltatime; // 小球1右移
        }

        // 控制小球2移动
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            balls[1].y -= speed * deltatime; // 小球2上移
        }
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            balls[1].x -= speed * deltatime; // 小球2左移
        }
        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            balls[1].y += speed * deltatime; // 小球2下移
        }
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            balls[1].x += speed * deltatime; // 小球2右移
        }

        // 更新小球位置
        for (auto& ball : balls)
        {
            ball.shape.setPosition(ball.x, ball.y);
        }

        // 玩家小球边界检测（替换原有的边界检测代码块）
        for (auto& ball : balls)
        {
            // 因为 setOrigin 设置为半径，位置代表圆心，所以边界为 [radius, width-radius] 和 [radius, height-radius]
            if (ball.x < ball.radius)
                ball.x = ball.radius;
            if (ball.y < ball.radius)
                ball.y = ball.radius;
            if (ball.x > desktop.width - ball.radius)
                ball.x = desktop.width - ball.radius;
            if (ball.y > desktop.height - ball.radius)
                ball.y = desktop.height - ball.radius;
            ball.shape.setPosition(ball.x, ball.y);
        }

        // 玩家小球之间的碰撞检测
        if (balls.size() >= 2)
        {
            float dx = balls[0].x - balls[1].x;
            float dy = balls[0].y - balls[1].y;
            float distance = sqrt(dx * dx + dy * dy);
            float minDist = balls[0].radius + balls[1].radius;
            if (distance < minDist)
            {
                // 简单分离处理：将两个球推开到不重叠
                float overlap = 0.5f * (minDist - distance);
                float nx = dx / distance;
                float ny = dy / distance;
                balls[0].x += nx * overlap;
                balls[0].y += ny * overlap;
                balls[1].x -= nx * overlap;
                balls[1].y -= ny * overlap;
                balls[0].shape.setPosition(balls[0].x, balls[0].y);
                balls[1].shape.setPosition(balls[1].x, balls[1].y);
            }
        }

        // ----------- 新增：大球吞噬小球和得分 -----------
        for (int player = 0; player < 2; ++player)
        {
            auto it = smallballs.begin();
            while (it != smallballs.end())
            {
                // 玩家球圆心
                float bx = balls[player].x;           //bx和by是玩家小球的圆心坐标
                float by = balls[player].y;
                // 小球圆心
                float sx = it->getPosition().x;
                float sy = it->getPosition().y;
                float sr = it->getRadius();

                float dx = bx - sx;
                float dy = by - sy;
                float dist = sqrt(dx * dx + dy * dy);

                if (dist < balls[player].radius + sr && balls[player].radius>sr)
                {
                    // 吞噬小球
                    if (player == 0)
                        ++redScore;
                    else
                        ++blueScore;
                    // 增大玩家球体积
                    balls[player].radius += sr * 0.15f;
                    balls[player].shape.setRadius(balls[player].radius);
                    balls[player].shape.setOrigin(balls[player].radius, balls[player].radius);
                    it = smallballs.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
        if (isAlive)
        {
            // 逃离玩家
            bool escaping = false;
            float escapeDistx = 0.f; // 逃离距离阈值
            float escapeDisty = 0.f;
            for (int i = 0; i < 2; ++i)
            {
                float dx = aiBall.x - balls[i].x;
                float dy = aiBall.y - balls[i].y;
                float dist = sqrt(dx * dx + dy * dy);
                if (dist < 150.f && balls[i].radius>aiBall.radius)
                {
                    if (dist > 0.01f)
                    {
                        escapeDistx += dx / dist;
                        escapeDisty += dy / dist;
                        escaping = true;
                    }
                }
            }
            // 如果在逃跑状态，就按叠加后的方向移动
            if (escaping)
            {
                // 先归一化方向向量，避免叠加后速度变快
                float len = sqrt(escapeDistx * escapeDistx + escapeDisty * escapeDisty);
                if (len > 0.01f)
                {
                    escapeDistx /= len;
                    escapeDisty /= len;
                }

                // 逃跑速度可以比平时快一点，比如 150
                aiBall.x += escapeDistx * 150.f * deltatime;
                aiBall.y += escapeDisty * 150.f * deltatime;
            }
            // 自动吃最近小球（只有不在逃跑时才执行）
            else if (!smallballs.empty())
            {
                float minDist = 1e9f;
                size_t minIdx = 0;
                // ... 你原来的找食物逻辑 ...
            }
            // 自动吃最近小球
            if (!escaping && !smallballs.empty())
            {
                float minDist = 1e9f;
                size_t minIdx = 0;
                for (size_t i = 0; i < smallballs.size(); ++i)
                {
                    float sx = smallballs[i].getPosition().x;
                    float sy = smallballs[i].getPosition().y;
                    float dx = sx - aiBall.x;
                    float dy = sy - aiBall.y;
                    float dist = sqrt(dx * dx + dy * dy);
                    if (dist < minDist)
                    {
                        minDist = dist;
                        minIdx = i;
                    }
                }
                float sx = smallballs[minIdx].getPosition().x;
                float sy = smallballs[minIdx].getPosition().y;
                float dx = sx - aiBall.x;
                float dy = sy - aiBall.y;
                float dist = sqrt(dx * dx + dy * dy);
                if (dist > 1.0f)
                {
                    aiBall.x += (dx / dist) * 120.f * deltatime;
                    aiBall.y += (dy / dist) * 120.f * deltatime;
                }
            }
        }

        // 边界检测
        if (aiBall.x < 0)
            aiBall.x = 0;
        if (aiBall.y < 0)
            aiBall.y = 0;
        if (aiBall.x > desktop.width - 2 * aiBall.radius)
            aiBall.x = desktop.width - 2 * aiBall.radius;
        if (aiBall.y > desktop.height - 2 * aiBall.radius)
            aiBall.y = desktop.height - 2 * aiBall.radius;
        aiBall.shape.setPosition(aiBall.x, aiBall.y);

        // AI球吞噬小球
        auto it = smallballs.begin();
        while (it != smallballs.end())
        {
            float sx = it->getPosition().x;
            float sy = it->getPosition().y;
            float sr = it->getRadius();
            float dx = aiBall.x - sx;
            float dy = aiBall.y - sy;
            float dist = sqrt(dx * dx + dy * dy);
            if (dist < aiBall.radius + sr && aiBall.radius > sr)
            {
                aiBall.radius += sr * 0.15f;
                aiBall.shape.setRadius(aiBall.radius);
                aiBall.shape.setOrigin(aiBall.radius, aiBall.radius);
                it = smallballs.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // 玩家吃AI球
        for (int player = 0; player < 2; ++player)
        {
            float dx = balls[player].x - aiBall.x;
            float dy = balls[player].y - aiBall.y;
            float dist = sqrt(dx * dx + dy * dy);

            if (dist < balls[player].radius + aiBall.radius && balls[player].radius > aiBall.radius)
            {
                // 给予分数并只在首次吞噬时执行一次
                if (player == 0)
                    redScore += static_cast<int>(aiBall.radius);
                else
                    blueScore += static_cast<int>(aiBall.radius);

                // 增大玩家球体积（一次性）
                balls[player].radius += aiBall.radius * 0.2f;
                balls[player].shape.setRadius(balls[player].radius);
                balls[player].shape.setOrigin(balls[player].radius, balls[player].radius);

                // 标记 AI 已被吃掉并立即移除其交互/显示属性，防止重复触发
                isAlive = false;
                aiBall.radius = 0.f;
                aiBall.shape.setRadius(0.f);
                aiBall.shape.setOrigin(0.f, 0.f);
                aiBall.shape.setPosition(-10000.f, -10000.f); // 移出可视/碰撞区域

                break;
            }
        }

        // ----------- 新增：更新得分文本 -----------
        redText.setString("Red: " + to_string(redScore));
        blueText.setString("Blue: " + to_string(blueScore));
        // ----------------------------------------

        // 绘制小球
        window.clear(Color::Black); // 1. 先清屏

        for (const auto& ball : balls)
        {
            window.draw(ball.shape);
        }
        for (auto& ball : smallballs)
        {
            window.draw(ball);
        }
        window.draw(redText);
        window.draw(blueText);
        if (isAlive)
        {
            window.draw(aiBall.shape);
        }
        if (smallballs.empty())
        {
            // 居中显示 "GAME OVER"
            gameOverText.setString("GAME OVER");
            FloatRect gb = gameOverText.getLocalBounds();
            gameOverText.setOrigin(gb.left + gb.width / 2.f, gb.top + gb.height / 2.f);
            gameOverText.setPosition(static_cast<float>(desktop.width) / 2.f, static_cast<float>(desktop.height) / 2.f - 40.f);
            window.draw(gameOverText);

            // 计算并显示获胜者
            string winner;
            if (redScore > blueScore)
                winner = "RED WINS: " + to_string(redScore) + " - " + to_string(blueScore);
            else if (blueScore > redScore)
                winner = "BLUE WINS: " + to_string(blueScore) + " - " + to_string(redScore);
            else
                winner = "TIE: " + to_string(redScore) + " - " + to_string(blueScore);

            winnerText.setString(winner);
            FloatRect wb = winnerText.getLocalBounds();
            winnerText.setOrigin(wb.left + wb.width / 2.f, wb.top + wb.height / 2.f);
            winnerText.setPosition(static_cast<float>(desktop.width) / 2.f, static_cast<float>(desktop.height) / 2.f + 40.f);
            window.draw(winnerText);
        }
        window.display(); // 2. 最后显示
    }

    return 0;
}
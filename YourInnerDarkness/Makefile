CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
SRCS = main.cpp Game/Game.cpp Player/Player.cpp Enemy/Enemy.cpp Enemy/ShadowBoss.cpp Enemy/EnemyFactory.cpp Security/Security.cpp
TARGET = YourInnerDarkness

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o game.log

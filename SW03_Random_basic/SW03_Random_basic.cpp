#include <iostream>
#include <vector>
#include <random>

int main()
{
	//std::random_device rd;
	//std::mt19937 gen(rd());

	//for (int i = 0; i < 10; i++)
	//{
	//	std::cout << gen() % 10 << std::endl;
	//}

	//std::cout << "-----" << std::endl;
	//std::uniform_int_distribution<> dist(0, 9);
	//for (int i = 0; i < 10; i++)
	//{
	//	std::cout << dist(gen) << std::endl;
	//}

	std::vector<int> myDeck{ 1, 2, 3, 4, 5 };
	std::random_device rd;
	std::mt19937 gen(rd());

	std::shuffle(myDeck.begin(), myDeck.end(), gen);
	for (auto& e : myDeck)
	{
		std::cout << e << std::endl;
	}
}


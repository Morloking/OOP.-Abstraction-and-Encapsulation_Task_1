#include <iostream>
#include <fstream>
#include <exception>
#include <string>

class Address {
public:
	Address(std::string city, std::string street, int home, int apart) :
		city(city),
		street(street),
		home(home),
		apart(apart)
	{
	}

	std::string getFormatAddress() {
		std::string finalStr{ "" };
		finalStr = city + ", " + street + ", " + std::to_string(home) + ", " + std::to_string(apart);
		return finalStr;
	}
private:
	std::string city{ "" };
	std::string street{ "" };
	int home{ 0 };
	int apart{ 0 };
};

void readAndWrite() {
	int amountOfAddresses{ 0 }, successCreated{ 0 };//добавил счетчик success created

	std::ifstream fileIn("in.txt");
	if (!fileIn.is_open()) throw std::runtime_error("Не удалось открыть файл для чтения.\n");
	fileIn >> amountOfAddresses;
	Address** allAddr = nullptr;					//объявил заранее, чтобы использовать в блоке трай кэтч + обнулил


	try {											//добавил трай кэтч прямо в области видимости, чтобы в случае выбрасывания исключения не было утечки памяти
		allAddr = new Address* [amountOfAddresses];

		for (int i = 0; i < amountOfAddresses; ++i) {
			std::string city{ "" }, street{ "" };
			int home{ 0 }, apart{ 0 };
			fileIn >> city >> street >> home >> apart;
			allAddr[i] = new Address(city, street, home, apart);
			++successCreated;						//увеличил счетчик
		}

		std::ofstream fileOut("out.txt");
		if (!fileOut.is_open()) throw std::runtime_error("Не удалось открыть файл для записи.\n");
		fileOut << amountOfAddresses << "\n";
		for (int i = amountOfAddresses - 1; i >= 0; --i) {
			fileOut << allAddr[i]->getFormatAddress() << "\n";
		}
	}
	catch (...) {									//удаление в случае выброса исключения
		for (int i = 0; i < successCreated; ++i) {
			delete allAddr[i];
			
		}
		delete[] allAddr;
		throw;
	}
	for (int i = 0; i < amountOfAddresses; ++i) {	//удаление стандартное после того, как все отработало
		delete allAddr[i];
	}
	delete[] allAddr;
}



int main() {
	setlocale(LC_ALL, "RUSSIAN");
	try {
		readAndWrite();
	}
	catch (const std::exception& ex) {
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}
	return EXIT_SUCCESS;
}

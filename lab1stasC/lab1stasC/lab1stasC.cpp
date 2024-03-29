

#include "pch.h"

#include <iostream>
#include <fstream>
#include <string>
#include <locale.h>

struct map
{
	char simbvol;
	double frequency = 0;
};//структура, которая хранит букву и кол-во(частоту) ее встреч в тексте

void sortP(map*& arr, int size);
std::string readChapters(std::string name);
void writeChapters(std::string name, std::string text);
std::string code(std::string text, int shift);
int getNewCode(int code, int shift);
void createTableSimbvols(map*& tableSimbvols, int n, std::string text);
int getReversShift(map*& tableSimbvols1, map*& tableSimbvols2, int n);

int main()
{
	setlocale(LC_CTYPE, "rus");
	//путь к файлам  в которые будет производиться запись
	std::string name1 = "D:\\marina inst\\lab1 (1)\\lab1stasC\\glavaOne.txt";
	std::string name2 = "D:\\marina inst\\lab1 (1)\\lab1stasC\\glava1enc.txt";
	std::string name3 = "D:\\marina inst\\lab1 (1)\\lab1stasC\\glava1dec.txt";
	std::string name4 = "D:\\marina inst\\lab1 (1)\\lab1stasC\\voyna-i-mir-tom-1.txt";
	
	std::string textOfFirstChapter, textEnc, textDec, textAll;//текст первой главы, зашифрованный текст, расшифрованный текст и весь текст соответственно
	
	textOfFirstChapter = readChapters(name1);//читаем 1 главу
	textEnc = code(textOfFirstChapter, 1);//шифруем со сдвигом = 1

	writeChapters(name2, textEnc);//записываем в файл 
	
	textAll = readChapters(name4);//читаем весь файл
	map *chapterOne = new map[64], *allChapters = new map[64];//создаем 2 массива текстур на 64 символа(32 заглавные буквы + 32 строчные)
	
	createTableSimbvols(chapterOne, 64, textEnc);//собираем таблицу частоты символов по зашифрованной главе
	createTableSimbvols(allChapters, 64, textAll);//собираем по всему тексту
	
	textDec = code(textEnc, getReversShift(chapterOne, allChapters, 64));//расшифровываем текст путем сдвига в обратную сторону
	writeChapters(name3, textDec);//записываем расшифрованный текст


	//std::string test = "АБВабв";
	//std::cout << code(test, 31);
}

std::string readChapters(std::string name) {//функция читающая главы
	std::string text, line;

	std::ifstream in(name); // окрываем файл для чтения
	if (in.is_open())
	{
		while (getline(in, line))
		{
			text = text + line + " \n ";
		}
	}
	in.close();     // закрываем файл
	return text;
}

void writeChapters(std::string name, std::string text) {

	std::ofstream out(name.c_str()); // поток для записи
	if (!out)//смотрим открылся ли файл?
	{
		std::cout << "err\n";//в случае ошибки
	}
	else {
		std::cout << "Success\n";//в случае удачи
		out << text ;
	}

	out.close();     // закрываем файл
}

void createTableSimbvols(map*& tableSimbvols, int n, std::string text) {//создам таблицы частоты символов
	double len = 0;
	for (int i = 0; i < text.length(); i++) {//пробегаемся по всем русским символам и заносим их колличество в массив
		if ((int)text[i] < -1 && (int)text[i] > -65) {

			tableSimbvols[(int)text[i] + 64].frequency += 1;
			len++;//увеличиваем общее кол-во символов
		}
	}

	for (int i = 0; i < 64; i++) {
		tableSimbvols[i].simbvol = (char)i - 64;//присваиваем имена символов в заглавие структуры
		tableSimbvols[i].frequency = tableSimbvols[i].frequency / len;//делим кол-во конкретных символов на общее кол-во символов

		//std::cout << tableSimbvols[i].simbvol << " = " << tableSimbvols[i].frequency << std::endl;
	}
}

int getReversShift(map*& tableSimbvols1, map*& tableSimbvols2, int n) {//ищем обратный сдвиг
	sortP(tableSimbvols1, n);
	sortP(tableSimbvols2, n);

	return (int)tableSimbvols2[0].simbvol - (int)tableSimbvols1[0].simbvol;//вычитаем коды символов(первых, ибо мы отсортировали массивы), чтобы узнать насколько ите сдвинулись
}

void sortP(map*& arr, int size)//сортируем массив пузырьком по возрастанию
{
	map temp;

	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - i - 1; j++) {
			if (arr[j].frequency < arr[j + 1].frequency) {
				// меняем элементы местами
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

std::string code(std::string text, int shift) {//шифруем текст

	std::string newText = "";
	char buf;
	for (int i = 0; i < text.length(); i++)//Читаем строку по символам и сдвигаем каждый из них
	{
		buf = (char)getNewCode((int)text[i], shift);
		//std::cout << int(text[i]) << " " << text[i] << " " << getNewCode((int)text[i], shift) << std::endl;
		newText = newText + buf;
	}

	return newText;
}

int getNewCode(int code, int shift) {//сдвиг буквы, если сдвиг = 2, то а = в
	
	shift = shift % 32;//защита, чтобы небыло сдвига больше алфавита
	int newCode = code;//если символ не русский, то возвращается тот же символ
	if(code > -65 && code < 0){
		newCode = newCode + shift;//если символ русский - прибавляем сдвиг

		if (code > -33) {//строчная 
			//std::cout << "1 newCode = " << (char)newCode << " " << newCode << " old code = " << (char)code << " " << code << std::endl;
			if (newCode > -1) {
				newCode -= 32;
			}
			else if(code < -32){
				newCode += 32;
			}
			//std::cout << "2 newCode = " << (char)newCode << " " << newCode << " old code = " << (char)code << " " << code << std::endl;
		}
		else if (code < -32) {//заглавная
			//std::cout << "3 newCode = "<< (char)newCode << " " << newCode << " old code = "  << (char)code << " " << code << std::endl;
			if (newCode >= -32) {
				newCode -= 32;
			}
			else if (code < -64) {
				newCode += 32;
			}
			//std::cout << "4 newCode = " << (char)newCode << " " << newCode << " old code = " << (char)code << " " << code << std::endl;
		}
	}
	return newCode;//возвращаем новый символ
}
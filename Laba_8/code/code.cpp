#include <fstream>
#include <string>
#include <string_view>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <queue>
#include <filesystem>
#include <bitset>


int ROWS = 0;
int COLUMNS = 0;
int GEN_COL = 0;
int USEFUL = 0;
int X = 0;// сколько всего бит

using my_value_t = std::pair<std::string, size_t>;// А: 32, ...
using my_container_t = std::vector<my_value_t>;

struct Node {
	std::string name;
	Node* parent;
	Node* left_child;
	Node* right_child;

	Node(std::string nm, Node* prt, Node* lchild = nullptr, Node* rchild = nullptr) :
		name(nm), parent(prt), left_child(lchild), right_child(rchild) {}

	Node(std::string nm) : name(nm), parent(nullptr), left_child(nullptr), right_child(nullptr) {}
};

// Класс Дерево Хаффмана
// Имеет стандартный набор основных методов:
// 
// make_tree - создать дерево Хаффмана, пока без кодов.
// code - кодировка входного файла
class HuffmanTree {
	Node* root = nullptr;
	std::vector<char> letters;

public:
	void get_codes(std::string name, Node* current_node, std::map<char, std::string>& table) {
		if ((current_node->left_child == nullptr) and (current_node->right_child == nullptr)) {
			table[current_node->name[0]] = name;
			return;
		}
		if (current_node->left_child) {
			get_codes(name + "0", current_node->left_child, table);
		}
		if (current_node->right_child) {
			get_codes(name + "1", current_node->right_child, table);
		}
	}

	void set_letters(const std::vector<char>& buf) 
	{
		letters.clear();
		letters.reserve(buf.size());
		letters.append_range(buf);
	}

	template <typename CMP>
	void make_tree(std::priority_queue<my_value_t, my_container_t, CMP>& q)
	{
		my_value_t left, right;
		size_t total_weight;
		Node* tmp_root = nullptr;
		Node* left_child;
		Node* right_child;
		std::map<std::string, Node*> storage;// хранит имя и указатель на вершину


		while (q.size() > 1)
		{
			// будем условно считать, что символ имеет "больший" приоритет
			// если встречается в тексте реже всего

			// извлекаем первый по приоритетности символ
			right = q.top();
			q.pop();

			// извлекаем второй по приоритетности символ
			left = q.top();
			q.pop();

			tmp_root = new Node(left.first + right.first);

			total_weight = right.second + left.second;
			right_child = storage.contains(right.first) ? storage[right.first] : new Node(right.first, tmp_root);
			left_child = storage.contains(left.first) ? storage[left.first] : new Node(left.first, tmp_root);

			// новую вершину (сумма двух предыдущих) -> кладем в очередь
			q.push(std::make_pair(left.first + right.first, total_weight));

			// формируем ветвь дерева
			tmp_root->left_child = left_child;
			tmp_root->right_child = right_child;

			storage.insert(std::make_pair(left.first + right.first, tmp_root));
		}
		root = tmp_root;
	}

	auto create_table(std::vector<my_value_t> q)
	{
		std::ofstream file_table;
		std::map<char, std::string> table;

		file_table.open("table.txt", std::ios::out);

		if (!file_table.is_open()) {
			std::cerr << "Ошибка открытия файла для таблицы!" << std::endl;
			exit(1);
		}

		get_codes("", root, table);

		std::cout << "\n";
		for (auto el : q) {
			//std::cout <<  el.first << " " << table[el.first[0]] << " " << el.second * (table[el.first[0]].size()) << "\n";
			X += el.second * (table[el.first[0]].size());
		}
		USEFUL = X % 8;
		//std::cout << "\n1 ----- X - " << X  << "USE - " << USEFUL;

		file_table << ROWS << std::endl;
		file_table << COLUMNS << std::endl;
		for (const auto& item : table) {
			std::cout << item.first << ":" << item.second << std::endl;
			file_table << item.first << ":" << item.second << std::endl;
		}
		file_table << X;
		file_table.close();
		return table;
	}

	void code(std::string flow, std::map<char, std::string>& table) {
		std::ofstream file_b;//бинарник для записи

		file_b.open("coded.bin", std::ios::binary);

		// пакуем биты в байты
		std::vector<uint8_t> packed_data;
		uint8_t byte = 0;
		int bit_count = 0;

		//кодируем

		std::string coded_flow;

		// кодируем количество полезных бит
		int A = 0;
		A = USEFUL;
		while (coded_flow.length() < 8) {
			if (USEFUL == 0) {
				coded_flow += "0";
				continue;
			}
			coded_flow += std::to_string((USEFUL % 2));
			USEFUL /= 2;
		}
		std::reverse(coded_flow.begin(), coded_flow.end());
		//USEFUL = A;

		int i = 0;
		for (int ch : flow) {
			i++;
			coded_flow += table[ch];
		}

		std::cout << "Данные из бинарного файла:\n";
		std::cout << "\n";

		for (int i = 0; i < coded_flow.length(); i++) {
			if ((i % 8) == 0) std::cout << " ";
			std::cout << coded_flow[i];
		}

		if ((coded_flow.length() % 8) != 0) {
			for (int i = 0; i < (8 - (coded_flow.length() % 8)); i++) {
				std::cout << "0";
			}
		}
		
		std::cout << "\n";

		byte = 0;

		for (char bit : coded_flow) {
			byte = (byte << 1) | (bit - '0'); // сдвигаем влево на бит и добавляем бит
			bit_count++;
			if (bit_count == 8) {
				packed_data.push_back(byte);
				byte = 0;
				bit_count = 0;
			}
		}

		// если остались "незаполненные" биты, дополняем их нулями
		if (bit_count > 0) {
			byte <<= (8 - bit_count);
			packed_data.push_back(byte);
		}

		// записываем упакованные данные в бинарный файл
		std::ofstream file("coded.bin", std::ios::binary);
		if (file.is_open()) {
			file.write(reinterpret_cast<const char*>(packed_data.data()), packed_data.size());
			file.close();
		}

		file_b.close();
	}

};

int main() {
	setlocale(LC_ALL, "rus");

	std::vector<my_value_t> copy;

	std::string filename = "test_2.txt";

	std::string data;
	std::ifstream file;//только чтение
	std::string gen_string;// строка со всеми элементами МАТРИЦЫ

	std::vector<char> all_letters;

	std::map<char, size_t> letters;
	std::vector<std::pair<char, size_t>> letters_vec;
	auto comp = [](const my_value_t& nd_1, const my_value_t& nd_2) {return nd_1.second > nd_2.second; };

	std::priority_queue<my_value_t, my_container_t, decltype(comp)> letters_q{ comp };
	
	file.open(filename, std::ios::in);
	if (!file.is_open()) {
		std::cerr << "Ошибка открытия файла в самом начале!" << std::endl;
		exit(-1);
	}

	while (getline(file, data)) {
		if (ROWS == 0) {
			if (isdigit(data[0])) {
				ROWS = std::atoi(data.c_str());
			}
		}
		else if (COLUMNS == 0) {
			if (isdigit(data[0])) {
				COLUMNS = std::atoi(data.c_str());
			}
		}
		else {
			for (int i = 0; i < data.size(); ++i) {
				if (letters.contains(data[i]))
					letters[data[i]]++;
				else
					letters[data[i]] = 1;
				GEN_COL++;
			}
			gen_string += data;
		}
	}

	//std::cout << "\ngen --- " << gen_string << "\n";

	for (const auto& item : letters){
		letters_q.push(std::make_pair(std::string(1, item.first), item.second));
		copy.push_back(std::make_pair(std::string(1, item.first), item.second));
		all_letters.push_back(item.first);
	}

	HuffmanTree tree;
	tree.make_tree(letters_q);
	tree.set_letters(all_letters);

	// начинаем кодирование
    data.clear();// буфер под строки матрицы

	file.close();

	auto table = tree.create_table(copy);

	tree.code(gen_string, table);


	// декодирование
	// открываем файл для чтения в бинарном режиме
	std::ifstream file_out_b("coded.bin", std::ios::binary);

	if (!file_out_b.is_open()) {
		std::cerr << "Не удалось открыть файл для чтения." << std::endl;
		return 1;
	}

	// считываем содержимое файла в вектор байтов
	std::istreambuf_iterator<char> start{ file_out_b }, end;

	std::vector<uint8_t> packed_data(start, end);
	file_out_b.close();

	// распаковываем байты в строку битов
	std::string binary_data_1;
	std::string binary_data_2;
	int cn = 0;
	int byte_in_f = X / 8;// кол-во неполных байт 
	std::string local_st;

	std::cout << "\nbyte_in_f --- " << byte_in_f << " -- количество целых байт в файле за исключением метаинформации\n";

	std::cout << "\nИнформация прочитанная из бинарного файла:\n";

	int USE = 0;
	for (uint8_t byte : packed_data) {
		// используем std::bitset для получения строкового представления битов
		if (cn == 0) {
			binary_data_1 += (std::bitset<8>(byte).to_string()); // преобразует каждый байт в строку из 8 символов
			//получаем десятичную запись
			std::reverse(binary_data_1.begin(), binary_data_1.end());
			for (int i = 0; i < binary_data_1.length(); i++) {
				if (binary_data_1[i] == '1') USE += pow(2, i);
			}
			std::reverse(binary_data_1.begin(), binary_data_1.end());
			std::cout << "\nЧисло полезных бит считанное из файла -- " << USE << "\n";
		}
		else {
			if ((byte_in_f + 1) == cn) {
				local_st = (std::bitset<8>(byte).to_string());
				for (int i = 0; i < USE; i++) {
					binary_data_2 += local_st[i];
				}
				break;
			}
			binary_data_2 += (std::bitset<8>(byte).to_string());
		}
		cn++;
	}

	std::cout << "\nbinary_1 - " << binary_data_1 <<  " -- байт хранящий полезное количество бит\n";

	std::string buff;
	
	for (int i = 0; i < binary_data_2.length(); i++) {
		if ((i % 8) == 0) std::cout << " ";
		std::cout << binary_data_2[i];
	}
	
	std::cout << "\n";

	for (auto el : binary_data_2) {
		buff += el;
		for (auto [key, val] : table) {
			if (val == buff) {
				std::cout << key;
				buff.clear();
			}
		}
	}
}

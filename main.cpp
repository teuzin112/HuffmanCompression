#include "caracter.h"
using namespace std;

int main() {
  setlocale(LC_ALL, "pt_BR.utf8");
  int opt;
  string nmArquivo;
  do {
    system("cls");
    printf("[1] - Compression\n");
    printf("[2] - Decompression\n");
    printf("[0] - Exit\n");
    printf("Option: ");
    scanf("%d", &opt);
    if (opt == 1) {
      printf("File name: ");
      std::cin >> nmArquivo;
      map<wchar_t, unsigned int> freq = readFile(nmArquivo.c_str());
      MinHeapNode* root = HuffmanCodes(freq);
      printCodes(root, "");
      compress(freq, root, (char*)nmArquivo.c_str());
      system("pause");
    }
    // Descompressao
    else if (opt == 2) {
      printf("File Name: ");
      std::cin >> nmArquivo;
      decompress((char*)nmArquivo.c_str());
      system("pause");
    }

  } while (opt != 0);
  return 0;
}

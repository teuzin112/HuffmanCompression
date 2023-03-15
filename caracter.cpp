#include "caracter.h"

// Mapa para armazenar todos os codigos e todos os caracteres
map<wchar_t, string> codes;

// Estrutura auxiliar utilizada como parametro para ordenacao da fila de nos
struct compare {
  bool operator()(MinHeapNode* l, MinHeapNode* r) {
    return (l->freq > r->freq);
  }
};

//  Funcao para imprimir todos os codigos da arvore de Huffman
//  @param root Raiz da arvore para impressao
//  @param str Variavel auxiliar para impressao dos codigos que deve ser chamar inicialmente com ""
void printCodes(struct MinHeapNode* root, string str) {
  if (!root)
    return;
  if (root->data != L'\xc386')
    cout << root->data << ": " << str << "\n";
  printCodes(root->left, str + "0");
  printCodes(root->right, str + "1");
}

// Funcao utilitaria para armazenar os codigos
// dos caracteres em uma variavel global do tipo mapa.
// @param root Raiz da arvore ja montada
// @param str
void storeCodes(struct MinHeapNode* root, string str) {
  if (root == NULL)
    return;
  if (root->data != L'\xc386')
    codes[root->data] = str;
  storeCodes(root->left, str + "0");
  storeCodes(root->right, str + "1");
}

// Funcao para montar a arvore de huffman
// e armazenar seus codigos.
// @param freq Mapa com todas as frequencias de todos os caracteres
// @return nova raiz da arvore montada
MinHeapNode* HuffmanCodes(map<wchar_t, unsigned int> freq) {
  priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare>
      minHeap;

  struct MinHeapNode *left, *right, *top;
  for (map<wchar_t, unsigned int>::iterator v = freq.begin(); v != freq.end(); v++)
    minHeap.push(new MinHeapNode(v->first, v->second));
  while (minHeap.size() != 1) {
    left = minHeap.top();
    minHeap.pop();
    right = minHeap.top();
    minHeap.pop();
    top = new MinHeapNode(L'\xc386',
                          left->freq + right->freq);
    top->left = left;
    top->right = right;
    minHeap.push(top);
  }
  storeCodes(minHeap.top(), "");

  return minHeap.top();
}

// Funcao para ler o arquivo e calcular a frequencia de todos
// os caracteres
// @param fileName Nome do arquivo a ser lido
// @return Mapa com todas as frequencias
map<wchar_t, unsigned int> readFile(const char* fileName) {
  FILE* iFile = fopen(fileName, "r");

  if (iFile == NULL) {
    cout << "Error while trying to open file" << endl;
    exit(-1);
  }

  wchar_t ch;
  map<wchar_t, unsigned int> freq;

  while ((ch = fgetwc(iFile)) != WEOF) {
    // putwchar(ch);
    freq[ch]++;
  }

  fclose(iFile);
  return freq;
}

// Funcao para comprimir o arquivo de texto
// @param fre Frequencia de todos os caracteres do arquivo
// @param root Raiz da arvore de huffman correspondente
void compress(map<wchar_t, unsigned int> freq, MinHeapNode* root, char* fileName) {
  string s = fileName;
  s = s.erase(s.length() - 4, 4);

  // Cria arquivo binario para compactacao
  FILE* oFile = fopen((s + "C.bin").c_str(), "wb");

  if (oFile == NULL) {
    cout << "Error while trying to open file" << endl;
    exit(-1);
  }

  // Pula 8 bytes do inicio do arquivo que sao reservados para escrita futura
  fseek(oFile, sizeof(lli), SEEK_SET);

  // Escreve o numero total de caracteres
  unsigned int totalCharacters = freq.size();
  fwrite(&totalCharacters, sizeof(unsigned int), 1, oFile);

  // Escreve os caracteres e suas respectivas frequencias no inicio do arquivo
  for (auto v = freq.begin(); v != freq.end(); v++) {
    fwrite(&v->first, sizeof(wchar_t), 1, oFile);
    fwrite(&v->second, sizeof(int), 1, oFile);
  }

  // Abre o arquivo de entrada para ler caracter por caracter e escrever o seu codigo de Huffman
  // correspondente no arquivo compactado
  FILE* iFile = fopen(fileName, "r");

  if (iFile == NULL) {
    cout << "Error while trying to open file" << endl;
    exit(-1);
  }

  lli codeLenght = 0;
  string strBits = "";
  wchar_t ch = fgetwc(iFile);

  while (ch != WEOF) {
    string cur = codes[ch];
    while (!cur.empty()) {
      // Remove o primeiro caracter da cadeia de bits do caractere lido e insere em strBits.
      strBits += cur[0];
      cur = cur.erase(0, 1);

      // Quando strBits atinge tamanho de 64 caracteres, converte-se para um long long int
      // e escreve 64 bits de uma vez no arquivo compactado.
      if (strBits.length() == 64) {
        lli bitsSequence = stoull(strBits, 0, 2);
        fwrite(&bitsSequence, sizeof(lli), 1, oFile);
        strBits = "";
        codeLenght += 64;
      }
    }
    ch = fgetwc(iFile);
  }
  // length armazena o numero exato de bits que foram escritos na compressao
  codeLenght += strBits.length();

  // "Arredonda" a ultima sequencia de bits caso necessario com zeros a direita
  if (!strBits.empty()) {
    while (strBits.length() < 64)
      strBits.insert(0, "0");
    lli lastBitsSequence = stoull(strBits, 0, 2);
    fwrite(&lastBitsSequence, sizeof(lli), 1, oFile);
  }
  fseek(oFile, 0, SEEK_SET);
  fwrite(&codeLenght, sizeof(lli), 1, oFile);

  fclose(oFile);
  fclose(iFile);
}

// Funcao utilitaria para converter um inteiro para uma sequencia de bits
// em binario do tipo string
// @param n Numero a ser convertido
// @return String convertida
string toBinary(lli n) {
  string r;
  while (n != 0) {
    r = (n % 2 == 0 ? "0" : "1") + r;
    n /= 2;
  }
  return r;
}

// Funcao para descomprimir um arquivo binario compactado por caractere
// @param fileName nome do arquivo a ser descompactado
void decompress(char* fileName) {
  FILE* compressedFile = fopen(fileName, "rb");
  fseek(compressedFile, 0, SEEK_SET);

  lli codeLength = 0;
  fread(&codeLength, sizeof(lli), 1, compressedFile);

  unsigned int totalCharacters;
  fread(&totalCharacters, sizeof(unsigned int), 1, compressedFile);

  map<wchar_t, unsigned int> freq;
  for (unsigned int i = 0; i < totalCharacters; i++) {
    wchar_t curChar;
    fread(&curChar, sizeof(wchar_t), 1, compressedFile);

    unsigned int curFreq;
    fread(&curFreq, sizeof(unsigned int), 1, compressedFile);

    freq[curChar] = curFreq;
  }

  codes.clear();
  MinHeapNode* root = HuffmanCodes(freq);

  string s = fileName;
  s = s.erase(s.length() - 4, 4);

  FILE* decompressedFile = fopen((s + "D.txt").c_str(), "w");
  int auxCodeLength = codeLength;
  MinHeapNode* cur = root;
  for (; auxCodeLength > sizeof(lli) * 8; auxCodeLength -= sizeof(lli) * 8) {
    lli bitsSequence;
    fread(&bitsSequence, sizeof(lli), 1, compressedFile);
    string bitsStr = toBinary(bitsSequence);

    int Aux = (sizeof(lli) * 8) - bitsStr.length();
    for (int i = 0; i < Aux; i++)
      bitsStr.insert(0, "0");

    while (!bitsStr.empty()) {
      if (cur->data != L'\xc386') {
        fputwc(cur->data, decompressedFile);
        cur = root;
        continue;
      }
      char c = bitsStr[0];
      bitsStr = bitsStr.erase(0, 1);

      if (c == '0')
        cur = cur->left;
      else
        cur = cur->right;
    }
    if (cur->data != L'\xc386') {
      fputwc(cur->data, decompressedFile);
      cur = root;
    }
  }

  lli lastBitsSequence;
  fread(&lastBitsSequence, sizeof(lli), 1, compressedFile);
  string lastBitsStr = toBinary(lastBitsSequence);

  int aux = auxCodeLength - lastBitsStr.length();
  for (int i = 0; i < aux; i++)
    lastBitsStr.insert(0, "0");

  while (!lastBitsStr.empty()) {
    if (cur->data != L'\xc386') {
      fputwc(cur->data, decompressedFile);
      cur = root;
      continue;
    }
    char c = lastBitsStr[0];
    lastBitsStr = lastBitsStr.erase(0, 1);

    if (c == '0')
      cur = cur->left;
    else
      cur = cur->right;
  }
  fputwc(cur->data, decompressedFile);
  fclose(compressedFile);
  fclose(decompressedFile);
}

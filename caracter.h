#include <bits/stdc++.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include <cstring>
#include <iostream>
#define lli unsigned long long int
using namespace std;

// No da arvore de huffman
struct MinHeapNode {
  wchar_t data;
  unsigned int freq;
  MinHeapNode *left, *right;

  MinHeapNode(wchar_t data, unsigned int freq) {
    left = right = NULL;
    this->data = data;
    this->freq = freq;
  }
};

//  Funcao para imprimir todos os codigos da arvore de Huffman
//  @param root Raiz da arvore para impressao
//  @param str Variavel auxiliar para impressao dos codigos que deve ser chamar inicialmente com ""
void printCodes(struct MinHeapNode* root, string str);

// Funcao utilitaria para armazenar os codigos
// dos caracteres em uma variavel global do tipo mapa.
// @param root Raiz da arvore ja montada
// @param str
void storeCodes(struct MinHeapNode* root, string str);

// Funcao para montar a arvore de huffman
// e armazenar seus codigos.
// @param freq Mapa com todas as frequencias de todos os caracteres
// @return nova raiz da arvore montada
MinHeapNode* HuffmanCodes(map<wchar_t, unsigned int> freq);

// Funcao para ler o arquivo e calcular a frequencia de todos
// os caracteres
// @param fileName Nome do arquivo a ser lido
// @return Mapa com todas as frequencias
map<wchar_t, unsigned int> readFile(const char* fileName);

// Funcao para comprimir o arquivo de texto
// @param fre Frequencia de todos os caracteres do arquivo
// @param root Raiz da arvore de huffman correspondente
void compress(map<wchar_t, unsigned int> freq, MinHeapNode* root, char* fileName);

// Funcao utilitaria para converter um inteiro para uma sequencia de bits
// em binario do tipo string
// @param n Numero a ser convertido
// @return String convertida
string toBinary(lli n);

// Funcao para descomprimir um arquivo binario compactado por caractere
// @param fileName nome do arquivo a ser descompactado
void decompress(char* fileName);

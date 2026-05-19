#include "../headers/model/ProductContainer.h"

// Inicializamos o contentor com espaço para 5 produtos (podes mudar este valor)
ProductContainer::ProductContainer() {
    capacity = 5;
    count = 0;
    productsArray = new Product[capacity];
}

// O Destrutor é OBRIGATÓRIO quando usamos 'new', para libertar a RAM do PC
ProductContainer::~ProductContainer() {
    delete[] productsArray;
}

// A função que salva a pátria quando o array enche
void ProductContainer::expand() {
    capacity *= 2; // Duplicamos o tamanho
    Product* newArray = new Product[capacity]; // Criamos um array novo maior

    // Copiamos os produtos antigos para o array novo
    for (int i = 0; i < count; i++) {
        newArray[i] = productsArray[i];
    }

    // Apagamos o array velho da memória e passamos a usar o novo
    delete[] productsArray;
    productsArray = newArray;
}

// Adicionar um produto
void ProductContainer::addProduct(const Product& product) {
    if (count == capacity) {
        expand(); // Se estiver cheio, manda alargar o array primeiro
    }
    productsArray[count] = product; // Adiciona na próxima posição vazia
    count++; // Aumenta o contador
}

// Procurar pelo ID (passou do teu antigo PescaTudo.cpp para aqui)
Product& ProductContainer::findById(int id) {
    for (int i = 0; i < count; i++) {
        if (productsArray[i].getId() == id) {
            return productsArray[i];
        }
    }
    throw std::runtime_error("Product not found.");
}

// Retorna quantos produtos existem (útil para os ciclos for)
int ProductContainer::getSize() const {
    return count;
}

// Permite aceder a um produto específico pela posição
Product& ProductContainer::getProduct(int index) {
    if (index < 0 || index >= count) {
        throw std::out_of_range("Index invalido.");
    }
    return productsArray[index];
}

const Product& ProductContainer::getProduct(int index) const {
    if (index < 0 || index >= count) {
        throw std::out_of_range("Index invalido.");
    }
    return productsArray[index];
}

void ProductContainer::removeProduct(int id) {
    int indexToRemove = -1;
    // Procurar a posição do produto
    for (int i = 0; i < count; i++) {
        if (productsArray[i].getId() == id) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1) {
        throw std::runtime_error("Product not found to delete.");
    }

    // Puxar todos os produtos à direita do apagado, uma posição para trás
    for (int i = indexToRemove; i < count - 1; i++) {
        productsArray[i] = productsArray[i + 1];
    }

    count--; // Reduz o tamanho do nosso array lógico
}
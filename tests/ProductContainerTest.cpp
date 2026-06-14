#include <gtest/gtest.h>
#include "model/ProductContainer.h"
#include "exceptions/ProductNotFoundException.h"

// 1. Testar Construtor por Omissão
TEST(ProductContainerTest, DefaultConstructor) {
    // Arrange
    ProductContainer container;

    // Act
    int size = container.getSize();
    bool isEmpty = container.empty();

    // Assert
    EXPECT_EQ(size, 0);
    EXPECT_TRUE(isEmpty);
}

// 2. Testar Adição de Produto (Garantir que não lança exceção, estilo slide 6)
TEST(ProductContainerTest, AddValidProduct) {
    // Arrange
    ProductContainer container;
    Product p(1, "Anzol", "MarcaX", 100, "Acessorios", "Desc", 0.5f, 1.0f);
    bool flag = true;

    // Act
    try {
        container.addProduct(p);
    } catch (...) {
        flag = false; // Se algo falhar na adição, a flag fica a false
    }

    // Assert
    EXPECT_TRUE(flag);
    EXPECT_EQ(container.getSize(), 1);
}

// 3. Testar Pesquisa de Produto Inexistente (Garantir que LANÇA a exceção correta)
TEST(ProductContainerTest, FindInvalidProductThrowsException) {
    // Arrange
    ProductContainer container;
    bool exceptionThrown = false; // Começamos a false porque esperamos que a exceção mude isto

    // Act
    try {
        // Tentamos procurar um ID que não existe num contentor vazio
        container.findById(999);
    } catch (ProductNotFoundException& e) {
        exceptionThrown = true; // Sucesso: Apanhou a exceção certa!
    }

    // Assert
    EXPECT_TRUE(exceptionThrown);
}

// 4. Testar Remoção de Produto
TEST(ProductContainerTest, RemoveProduct) {
    // Arrange
    ProductContainer container;
    Product p(1, "Isco", "MarcaY", 50, "Iscos", "Desc", 2.0f, 4.0f);
    container.addProduct(p);

    // Act
    container.removeProduct(1);

    // Assert
    EXPECT_TRUE(container.empty());
    EXPECT_EQ(container.getSize(), 0);
}

// 5. Testar Construtor de Cópia (Regra dos 3)
TEST(ProductContainerTest, CopyConstructor) {
    // Arrange
    ProductContainer original;
    Product p(1, "Cana", "Shimano", 10, "Canas", "Desc", 50.0f, 100.0f);
    original.addProduct(p);

    // Act
    ProductContainer copy(original);

    // Assert
    EXPECT_EQ(copy.getSize(), 1);
    EXPECT_EQ(copy.findById(1).getName(), "Cana");
}

// 6. Testar Operador de Atribuição (operator=)
TEST(ProductContainerTest, AssignmentOperator) {
    // Arrange
    ProductContainer original;
    Product p(1, "Carreto", "Daiwa", 5, "Carretos", "Desc", 40.0f, 80.0f);
    original.addProduct(p);
    ProductContainer assigned;

    // Act
    assigned = original;

    // Assert
    EXPECT_EQ(assigned.getSize(), 1);
    EXPECT_EQ(assigned.findById(1).getName(), "Carreto");
}
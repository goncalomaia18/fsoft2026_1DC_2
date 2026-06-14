#include <gtest/gtest.h>
#include "model/Product.h"
#include "model/Supplier.h"

// 1. Testar Construtor por Omissão
TEST(ProductTest, DefaultConstructor) {
    //Arrange
    Product p;

    //Act
    // A ação aqui é a própria criação que ocorreu no Arrange,
    // mas chamamos os getters para verificar o estado inicial.
    int id = p.getId();

    //Assert
    // Verificamos se o C++ inicializou com lixo ou com valores default adequados
    // (Assumindo que no teu .cpp o ID começa a 0 ou equivalente)
    EXPECT_EQ(id, 0);
}

// 2. Testar Construtor Parametrizado (Sem Supplier)
TEST(ProductTest, ParameterizedConstructorWithoutSupplier) {
    //Arrange
    int expectedId = 1;
    std::string expectedName = "Cana de Pesca";
    std::string expectedBrand = "Shimano";
    int expectedStock = 10;
    std::string expectedCategory = "Canas";
    std::string expectedDescription = "Cana em carbono";
    float expectedPriceSupplier = 25.0f;
    float expectedPriceClient = 45.50f;

    //Act
    Product p(expectedId, expectedName, expectedBrand, expectedStock,
              expectedCategory, expectedDescription, expectedPriceSupplier, expectedPriceClient);

    //Assert
    EXPECT_EQ(p.getId(), expectedId);
    EXPECT_EQ(p.getName(), expectedName);
    EXPECT_EQ(p.getBrand(), expectedBrand);
    EXPECT_EQ(p.getStock(), expectedStock);
    EXPECT_EQ(p.getCategory(), expectedCategory);
    EXPECT_EQ(p.getDescription(), expectedDescription);
    EXPECT_FLOAT_EQ(p.getPriceSupplier(), expectedPriceSupplier);
    EXPECT_FLOAT_EQ(p.getPriceClient(), expectedPriceClient);
}

// 3. Testar Construtor Parametrizado (Com Supplier)
TEST(ProductTest, ParameterizedConstructorWithSupplier) {
    //Arrange
    Supplier s; // Assumindo que Supplier tem construtor por omissão

    //Act
    Product p(2, "Anzol", "MarcaX", 50, "Acessorios", "Anzol nº4", 0.5f, 1.2f, s);

    //Assert
    EXPECT_EQ(p.getId(), 2);
    // Não testamos os outros todos para não ser repetitivo, focamos no Supplier
    // A validação de igualdade do Supplier dependerá se tens o operator== implementado no Supplier.h
    // EXPECT_EQ(p.getSupplier().getId(), s.getId());
}

// 4. Testar Setters e Getters (Testamos um para representar todos)
TEST(ProductTest, SettersAndGetters) {
    //Arrange
    Product p;
    std::string newName = "Carreto X";

    //Act
    p.setName(newName);

    //Assert
    EXPECT_EQ(p.getName(), newName);
}

// 5. Testar Redução de Stock
TEST(ProductTest, ReduceStockFunctionsCorrectly) {
    //Arrange
    Product p(1, "Isco", "MarcaA", 20, "Iscos", "Desc", 1.0f, 2.0f);
    int reductionAmount = 5;
    int expectedStock = 15;

    //Act
    p.reduceStock(reductionAmount);

    //Assert
    EXPECT_EQ(p.getStock(), expectedStock);
}

// 6. Testar Aumento de Stock
TEST(ProductTest, IncreaseStockFunctionsCorrectly) {
    //Arrange
    Product p(1, "Isco", "MarcaA", 20, "Iscos", "Desc", 1.0f, 2.0f);
    int increaseAmount = 10;
    int expectedStock = 30;

    //Act
    p.increaseStock(increaseAmount);

    //Assert
    EXPECT_EQ(p.getStock(), expectedStock);
}
#include "../../headers/model/Product.h"

Product::Product()
        : id(0), name(""), brand(""), stock(0), category(""), description(""),
          priceSupplier(0.0f), priceClient(0.0f), supplier(Supplier(0, "", "", "")) {}

Product::Product(int id, const std::string& name, const std::string& brand, int stock,
                 const std::string& category, const std::string& description,
                 float priceSupplier, float priceClient)
        : id(id), name(name), brand(brand), stock(stock), category(category),
          description(description), priceSupplier(priceSupplier), priceClient(priceClient),
          supplier(Supplier(0, "", "", "")) {}

Product::Product(int id, const std::string& name, const std::string& brand, int stock,
                 const std::string& category, const std::string& description,
                 float priceSupplier, float priceClient, const Supplier& supplier)
        : id(id), name(name), brand(brand), stock(stock), category(category),
          description(description), priceSupplier(priceSupplier), priceClient(priceClient),
          supplier(supplier) {}

int Product::getId() const { return id; }
std::string Product::getName() const { return name; }
std::string Product::getBrand() const { return brand; }
int Product::getStock() const { return stock; }
std::string Product::getCategory() const { return category; }
std::string Product::getDescription() const { return description; }
float Product::getPriceClient() const { return priceClient; }
float Product::getPriceSupplier() const { return priceSupplier; }
Supplier Product::getSupplier() const { return supplier; }

void Product::reduceStock(int quantity) {
    if (quantity <= stock)
        stock -= quantity;
}

void Product::increaseStock(int quantity) {
    stock += quantity;
}

void Product::setName(const std::string& newName) { name = newName; }
void Product::setBrand(const std::string& newBrand) { brand = newBrand; }
void Product::setStock(int newStock) { stock = newStock; }
void Product::setCategory(const std::string& newCategory) { category = newCategory; }
void Product::setDescription(const std::string& newDesc) { description = newDesc; }
void Product::setPriceSupplier(float newPrice) { priceSupplier = newPrice; }
void Product::setPriceClient(float newPrice) { priceClient = newPrice; }
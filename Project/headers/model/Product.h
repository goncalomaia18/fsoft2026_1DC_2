//
// Created by cmgm on 16/05/2026.
//

#ifndef FSOFT2026_1DC__PRODUCT_H
#define FSOFT2026_1DC__PRODUCT_H
#include <string>
#include "Supplier.h"

class Product {
private:
    int id;
    std::string name;
    std::string brand;
    int stock;
    std::string category;
    std::string description;
    float priceSupplier;
    float priceClient;
    Supplier supplier;

public:
    Product();
    Product(int id, const std::string& name, const std::string& brand, int stock,
            const std::string& category, const std::string& description,
            float priceSupplier, float priceClient);
    Product(int id, const std::string& name, const std::string& brand, int stock,
            const std::string& category, const std::string& description,
            float priceSupplier, float priceClient, const Supplier& supplier);

    int getId() const;
    std::string getName() const;
    std::string getBrand() const;
    int getStock() const;
    std::string getCategory() const;
    std::string getDescription() const;
    float getPriceClient() const;
    float getPriceSupplier() const;
    Supplier getSupplier() const;

    void reduceStock(int quantity);
    void increaseStock(int quantity);
    void setName(const std::string& newName);
    void setBrand(const std::string& newBrand);
    void setStock(int newStock);
    void setCategory(const std::string& newCategory);
    void setDescription(const std::string& newDesc);
    void setPriceSupplier(float newPrice);
    void setPriceClient(float newPrice);
};

#endif //FSOFT2026_1DC__PRODUCT_H

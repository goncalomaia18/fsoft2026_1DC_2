cmake_minimum_required(VERSION 3.26)
project(fsoft2026_1DC_)

set(CMAKE_CXX_STANDARD 17)

add_executable(fsoft2026_1DC_
        main.cpp

        Project/mock/MockData.cpp

        Project/sources/controllers/Controller.cpp

        Project/sources/model/Cart.cpp
        Project/sources/model/Client.cpp
        Project/sources/model/ClientOrder.cpp
        Project/sources/model/Manager.cpp
        Project/sources/model/PescaTudo.cpp
        Project/sources/model/Product.cpp
        Project/sources/model/Supplier.cpp
        Project/sources/model/SupplierOrder.cpp


        Project/sources/views/View.cpp

        Project/sources/views/Utils.cpp
        Project/sources/exceptions/EmptyCartException.cpp
        Project/sources/exceptions/InvalidLoginException.cpp
        Project/sources/exceptions/OrderNotFoundException.cpp
        Project/sources/exceptions/ProductNotFoundException.cpp
        Project/sources/model/ProductContainer.cpp
        Project/sources/model/ClientContainer.cpp
        Project/sources/model/SupplierContainer.cpp
        Project/sources/model/SuppierOrderContainer.cpp
)

target_include_directories(fsoft2026_1DC_ PRIVATE
        Project/headers
        Project/mock
        Project/sources
)
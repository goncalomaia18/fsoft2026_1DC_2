#include "Project/headers/model/PescaTudo.h"
#include "Project/headers/controllers/Controller.h"
#include "Project/mock/MockData.h"

int main() {

    PescaTudo store;

    MockData mock;
    mock.generateData(store);

    Controller controller(store);

    // Adicionar produtos ao carrinho do cliente
    Product canaProd = store.findProductById(1);
    Product carretoProd = store.findProductById(2);

    controller.addProductToClientCart(
            "raul@gmail.com",
            canaProd,
            1
    );

    controller.addProductToClientCart(
            "raul@gmail.com",
            carretoProd,
            1
    );

    controller.run();

    return 0;
}
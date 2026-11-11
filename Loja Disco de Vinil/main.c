#include <stdio.h>
#include <locale.h>
#include "utilitarios.h"
#include "disco.h"
#include "cliente.h"
#include "venda.h"
#include "relatorio.h"

int main() {
    int opcao;

    setlocale(LC_ALL, "Portuguese");     
    setlocale(LC_NUMERIC, "C");          

    
    carregarDiscosCSV();
    carregarClientesCSV();
    carregarVendasCSV();
    pausar();

    do {
        limparTela();
        printf("=========== LOJA DE DISCOS DE VINIL ===========\n");
        printf("1 - Discos\n");
        printf("2 - Clientes\n");
        printf("3 - Vendas\n");
        printf("4 - Relatorios\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: menuDiscos();     break;
            case 2: menuClientes();   break;
            case 3: menuVendas();     break;
            case 4: menuRelatorios(); break;
            case 0: printf("Encerrando o sistema...\n"); break;
            default: printf("Opcao invalida!\n"); pausar();
        }

    } while (opcao != 0);

    return 0;
}

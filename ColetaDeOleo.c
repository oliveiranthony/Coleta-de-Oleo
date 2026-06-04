#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
typedef struct {
    char name[100], cpf[15], phone[20];
} Cadaster;

typedef struct {
    unsigned int quantity;
    float liters;
} Register;

void DateTime(char *buffer, size_t sizeMax) {
    time_t moment;
    struct tm *infoTime;
    time(&moment);
    infoTime = localtime(&moment);
    strftime(buffer, sizeMax, "%d/%m/%Y %H:%M:%S", infoTime);
}

void ClearBuffer() {
    int trash;
    while ((trash = getchar()) != '\n' && trash != EOF)
        ;
}

int SearchDonor(Cadaster *cad, char search[]){
    FILE *cadasterFile = fopen("CadastroDoador.txt", "r");

    if (cadasterFile == NULL) {
        return 0;
    }

    char line[200];

    while (fgets(line, sizeof(line), cadasterFile)) {
        if (strncmp(line, "Nome: ", 6) == 0) {
            strcpy(cad->name, line + 6);
            cad->name[strcspn(cad->name, "\n")] = '\0';
        }

        else if (strncmp(line, "CPF: ", 5) == 0) {
            strcpy(cad->cpf, line + 5);
            cad->cpf[strcspn(cad->cpf, "\n")] = '\0';

            if (strcmp(search, cad->name) == 0 ||
                strcmp(search, cad->cpf) == 0) {

                fclose(cadasterFile);
                return 1;
            }
        }
    }

    fclose(cadasterFile);
    return 0;
}

int TotalDonatedPerUser(Cadaster *cad, char searchUser[]) {
    FILE *reportFilePerUser = fopen("RegistroDoacao.txt", "r");
    char line[200];
    float totalLiters = 0.0, donationValue;
    unsigned int totalBottles = 0, bottles;

    if (reportFilePerUser == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return 0;
    }

    if (SearchDonor(cad, searchUser))  {
        while (fgets(line, sizeof(line), reportFilePerUser)) {
            if (strncmp(line, "Quantidade de garrafas: ", 24) == 0) {
                sscanf(line + 24, "%u", &bottles);

                if (strcmp(searchUser, cad->name) == 0) {
                    totalBottles += bottles;
                }
            }
            else if (strncmp(line, "Total doado neste registro: ", 28) == 0) {
                sscanf(line + 28, "%f", &donationValue);

                if (strcmp(searchUser, cad->name) == 0) {
                    totalLiters += donationValue;
                }
            }
        }
    }

    fclose(reportFilePerUser);

    printf("\n===== RELATORIO DO USUARIO =====\n");
    printf("Doador: %s\n", searchUser);
    printf("Total de garrafas doadas: %u\n", totalBottles);
    printf("Total de litros doados: %.2f\n", totalLiters);
    return 1;
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    FILE *cadasterFile, *registerFile, *reportFilePerUser, *reportFilePerDay, *totalFile;
    cadasterFile = fopen("CadastroDoador.txt", "a");
    registerFile = fopen("RegistroDoacao.txt", "a");
    Cadaster cad;
    Register reg;
    int option = 0;

    while (option < 1 || option > 5) {
        printf("===== SISTEMA DE DOACAO DE OLEO =====\n");
        printf("1 - Cadastrar doador.\n");
        printf("2 - Registrar doacao.\n");
        printf("3 - Relatorios.\n");
        printf("4 - Total arrecadado.\n");
        printf("5 - Sair... .\n");
        printf("Digite a opcao desejada: ");

        scanf("%d", &option);
        ClearBuffer();
    }
    fclose(cadasterFile);

    switch (option) {
    case 1:
        cadasterFile = fopen("CadastroDoador.txt", "a+");
        if (cadasterFile == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            return 1;
        }
        printf("\nCadastro de doador:\n");

        printf("Insira seu nome completo: ");
        fgets(cad.name, 100, stdin);
        cad.name[strcspn(cad.name, "\n")] = '\0';

        printf("Informe seu cpf: ");
        fgets(cad.cpf, 15, stdin);
        cad.cpf[strcspn(cad.cpf, "\n")] = '\0';
        ClearBuffer();

        printf("Telefone de registro: ");
        fgets(cad.phone, 20, stdin);
        cad.phone[strcspn(cad.phone, "\n")] = '\0';

        fprintf(cadasterFile, "Nome: %s\n"
                              "CPF: %s\n"
                              "Telefone: %s\n",
                cad.name, cad.cpf, cad.phone);
        char currentDate[20];
        DateTime(currentDate, sizeof(currentDate));
        fprintf(cadasterFile, "Momento do cadastro: %s\n\n", currentDate);
        fclose(cadasterFile);
        printf("Cadastro realizado com sucesso!\n");
        break;

    case 2:
        char search[100];
        float totalPerRegister = 0.0;
        registerFile = fopen("RegistroDoacao.txt", "a+");
        printf("\nRegistro de doacao:\n");
        printf("Buscar doador (Nome ou CPF): ");
        fgets(search, 100, stdin);
        search[strcspn(search, "\n")] = '\0';
        if (!SearchDonor(&cad, search)){
            printf("Doador nao encontrado. Por favor, cadastre-se primeiro.\n");
            break;
        }
        printf("Doador encontrado!\n");

        printf("Quantidade de garrafas doadas: ");
        scanf("%u", &reg.quantity);
        ClearBuffer();
        printf("Informe a capacidade da garrafa (em litros): ");
        scanf("%f", &reg.liters);
        ClearBuffer();
        totalPerRegister = reg.liters * reg.quantity;
        printf("Total doado neste registro: %.2f litros\n", totalPerRegister);
        fprintf(registerFile, "Doador: %s\n"
                              "Quantidade de garrafas: %u\n"
                              "Capacidade da(s) garrafa(s) doada(s): %.2f litros\n"
                              "Total doado neste registro: %.2f litros\n",
                cad.name, reg.quantity, reg.liters, totalPerRegister);

        DateTime(currentDate, sizeof(currentDate));
        fprintf(registerFile, "Doação realizada em: %s\n\n", currentDate);
        fclose(registerFile);
        break;

    case 3:
        printf("\n===== Relatorios =====\n");
        printf("1 - Relatorio por usuario.\n");
        printf("2 - Relatorio por dia.\n");
        printf("Informe o relatorio: ");
        scanf("%d", &option);
        ClearBuffer();

        switch (option) {
        case 1:
            char searchUser[100], currentDate[20];
            reportFilePerUser = fopen("RelatorioPorUsuario.txt", "a+");
            DateTime(currentDate, sizeof(currentDate));

            printf("Digite o nome do doador: ");
            fgets(searchUser, sizeof(searchUser), stdin);
            searchUser[strcspn(searchUser, "\n")] = '\0';
            if (!TotalDonatedPerUser(&cad, searchUser)) {
                puts("Erro ao gerar relatorio por usuario.");
                printf("Certifique-se de que o doador foi cadastrado e que ha registros de doacoes para ele.\n");
                break;
            }

            fprintf(reportFilePerUser, "Relatório do usuário: %s\n"
                                        "Gerado em: %s\n",
                                         searchUser, currentDate);

            fclose(reportFilePerUser);
            break;

        case 2:
            registerFile = fopen("RegistroDoacao.txt", "r");
            if (registerFile == NULL) {
                printf("Erro ao abrir o arquivo de registro.\n");
                return 1;
            }

            break;

        default:
            printf("Opção inválida.");
            break;
        }
        break;

    case 4:
        totalFile = fopen("registro.txt", "r+");
        printf("\nTotal arrecadado:\n");
        break;

    case 5:
        printf("\nSaindo do sistema...\n");
        return 1;
    }

    return 0;
}
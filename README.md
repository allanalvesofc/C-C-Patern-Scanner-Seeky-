# 🔍 Localizador e Extrator de Linhas em Arquivos Grandes

## 🇧🇷 Descrição em Português

### Visão Geral
Este software é uma ferramenta de **busca e extração de linhas contendo um termo específico** em arquivos de texto muito grandes (até vários gigabytes), utilizando **mapeamento de arquivos em memória** para alto desempenho.  
Ele processa o arquivo por blocos, evitando sobrecarregar a memória, e salva todas as linhas correspondentes em um arquivo `.txt` separado.

---

### Principais Funcionalidades
- **Busca em Arquivos Massivos**  
  Capaz de processar arquivos de até **5 GB por bloco mapeado**, ideal para grandes bases de dados.

- **Entrada Flexível**  
  - Aceita argumentos de linha de comando:  
    ```
    programa.exe <nome_arquivo> <termo_busca>
    ```
  - Ou entrada interativa, solicitando nome do arquivo e termo de busca.

- **Mapeamento de Arquivos em Memória**  
  Utiliza `CreateFileMapping` e `MapViewOfFile` para ler o arquivo diretamente da memória, otimizando velocidade.

- **Busca Personalizada**  
  Algoritmo que localiza **todas as ocorrências** do termo exato no arquivo.

- **Extração de Linhas Completas**  
  Para cada ocorrência encontrada, extrai a linha inteira onde o termo aparece.

- **Saída Organizada**  
  Cria um arquivo `<termo>.txt` contendo todas as linhas encontradas e um resumo com o número total de resultados.

- **Abertura Automática do Resultado**  
  Ao final da execução, abre o arquivo gerado no visualizador padrão do sistema.

---

### Fluxo de Funcionamento
1. O usuário informa o arquivo e o termo de busca.
2. O programa abre o arquivo no modo somente leitura.
3. Mapeia partes do arquivo na memória em blocos de até 5 GB.
4. Busca todas as ocorrências do termo em cada bloco.
5. Para cada ocorrência, extrai e salva a linha inteira.
6. Exibe no console:
   - Total de resultados
   - Tempo total de execução
7. Abre automaticamente o arquivo de saída.

---

### Requisitos
- **Sistema Operacional:** Windows (com suporte a WinAPI)
- **Compilador:** MSVC (Microsoft Visual C++) ou compatível
- **Bibliotecas Necessárias:**  
  - `windows.h`
  - `shellapi.h`
  - `<iostream>`, `<fstream>`, `<vector>`, `<string>`

---

### Possíveis Usos
- Busca rápida em **bases de dados exportadas** em formato texto.
- Extração de registros específicos em **logs de aplicações**.
- Filtragem de dados em **listas grandes** de informações.

---

## 🇺🇸 Description in English

### Overview
This software is a **search and extraction tool** for lines containing a specific term in extremely large text files (up to several gigabytes), using **memory-mapped files** for high performance.  
It processes the file in chunks, preventing memory overload, and saves all matching lines to a separate `.txt` file.

---

### Main Features
- **Massive File Search**  
  Can process files up to **5 GB per mapped chunk**, ideal for large datasets.

- **Flexible Input**  
  - Accepts command-line arguments:  
    ```
    program.exe <file_name> <search_term>
    ```
  - Or interactive mode, prompting for file name and search term.

- **Memory-Mapped File Access**  
  Uses `CreateFileMapping` and `MapViewOfFile` to read the file directly from memory for optimized speed.

- **Custom Search**  
  Algorithm locates **all exact matches** of the search term in the file.

- **Full Line Extraction**  
  For each match, extracts the entire line where the term appears.

- **Organized Output**  
  Creates a `<term>.txt` file containing all matching lines and a summary with the total results.

- **Automatic Result Opening**  
  After execution, automatically opens the output file in the default viewer.

---

### Workflow
1. User provides the file and search term.
2. Program opens the file in read-only mode.
3. Maps portions of the file into memory in up to 5 GB chunks.
4. Searches for all occurrences of the term in each chunk.
5. Extracts and saves the full line for each match.
6. Displays:
   - Total results found
   - Total execution time
7. Automatically opens the output file.

---

### Requirements
- **Operating System:** Windows (with WinAPI support)
- **Compiler:** MSVC (Microsoft Visual C++) or compatible
- **Required Libraries:**  
  - `windows.h`
  - `shellapi.h`
  - `<iostream>`, `<fstream>`, `<vector>`, `<string>`

---

### Possible Use Cases
- Fast search in **exported database dumps**.
- Extracting specific records from **application logs**.
- Filtering data from **large text lists**.

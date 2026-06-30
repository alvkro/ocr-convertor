# Projeto EDB1: Binarização Otsu e Análise de Desempenho 📊

Este repositório contém a implementação completa do pipeline de binarização de imagens utilizando o método de Otsu, desenvolvido como requisito da disciplina de Estrutura de Dados Básicas I (EDB1).

⚠️ **ATENÇÃO:** O passo a passo abaixo é exclusivo para a etapa de **Análise Gráfica e Validação de Complexidade** (Geração dos gráficos em Python). Para instruções sobre como compilar e executar o código principal em C++ (geração do vetor dinâmico e processamento da imagem), consulte a documentação ou os comentários nos arquivos `.cpp`.

---

## 🛠️ Pré-requisitos para a Análise Gráfica

Para gerar os gráficos comparativos localmente, certifique-se de ter instalado:

- **Python 3** (versão 3.8 ou superior)
- **Pip** (gerenciador de pacotes do Python)

_(Nota: O arquivo de dados `dados_benchmark.csv` já deve ter sido gerado pela execução prévia do código em C++ e precisa estar na raiz desta pasta)._

---

## 🚀 Como gerar os gráficos (Execução Rápida)

Para evitar conflitos de bibliotecas no seu sistema operacional, a execução do gerador de gráficos deve ser feita isolada em um **Ambiente Virtual**.

Abra o terminal na pasta raiz do projeto e copie e cole o bloco de comandos correspondente ao seu sistema operacional:

### 🐧 Se você usa Linux ou macOS:

Copie e cole todas as linhas abaixo de uma só vez no terminal e aperte Enter:

```bash
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
python3 graph_generator.py
```

#### 🪟 Se você usa Windows:

Copie e cole todas as linhas abaixo de uma só vez no terminal e aperte Enter:

```bash
python -m venv venv
venv\Scripts\activate.bat
pip install -r requirements.txt
python3 graph_generator.py
```

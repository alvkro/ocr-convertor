import pandas as pd # ele quem sabe ler o arquivo de excel
import matplotlib.pyplot as plt # desenha o gráfico
import seaborn as sns # deixa o gráfico mais bonito

#Configuração visual
sns.set_theme(style="whitegrid")
plt.figure(figsize=(10, 6))

#Leitura do arquivo de dados gerado pelo C++ 
try:
    df = pd.read_csv('dados_benchmark.csv')
except FileNotFoundError:
    print("Erro: O arquivo 'dados_benchmark.csv' não foi encontrado na pasta.")
    exit()

#Plotagem da Linha 1: O Vetor Dinâmico da Equipe (Azul)
plt.plot(
    df['Total_Pixels'], 
    df['Tempo_NossoVetor_ms'], 
    label='Vetor Dinâmico (Equipe)', 
    color='#1f77b4',
    linewidth=2.5, 
    marker='o',
    markersize=6
)

#Plotagem da Linha 2: O std::vector original do C++ (Vermelho Tracejado)
plt.plot(
    df['Total_Pixels'], 
    df['Tempo_StdVector_ms'], 
    label='std::vector (C++ Nativo)', 
    color='#d62728',
    linewidth=2.5, 
    linestyle='--', 
    marker='s',
    markersize=6
)

# 5. Cabeçalhos e Textos do Gráfico
plt.title('Análise de Desempenho: Binarização Otsu (Vetor Customizado vs STL)', fontsize=14, fontweight='bold', pad=15)
plt.xlabel('Resolução da Imagem (Total de Pixels)', fontsize=12, labelpad=10)
plt.ylabel('Tempo de Execução (ms)', fontsize=12, labelpad=10)

# Formata o eixo X para não mostrar números científicos
plt.ticklabel_format(style='plain', axis='x')

# Configura a legenda
plt.legend(loc='upper left', fontsize=11, frameon=True, shadow=True)

# Exportação e Exibição
plt.tight_layout()
plt.savefig('grafico_desempenho_otsu.png', dpi=300)
print("Gráfico salvo com sucesso como 'grafico_desempenho_otsu.png'!")

plt.show() # Abre a janela para visualizar a imagem do gráfico gerado
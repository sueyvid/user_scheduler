# 🧵 User Scheduler com Docker

Este projeto é um escalonador de threads em espaço de usuário, escrito em C, e preparado para ser executado com Docker, facilitando a compilação e execução em diferentes sistemas operacionais (Linux ou Windows).

---

## 📦 Build da Imagem Docker

Antes de rodar o projeto, é necessário construir a imagem Docker a partir do `Dockerfile`:

```bash
docker build -t user-scheduler .
```

🔧 Isso cria uma imagem chamada `user-scheduler` contendo o compilador e todas as dependências necessárias para compilar e executar o programa C.

---

## 🚀 Executando o Container

### 🔷 No Linux:

```bash
docker run -it --rm -v "$PWD":/app user-scheduler
```

### 🟦 No Windows (PowerShell ou CMD):

```bash
docker run -it --rm -v ${PWD}:/app user-scheduler
```

🔍 Explicando os parâmetros:

- `-it`: inicia o container em modo interativo com terminal.
- `--rm`: remove o container automaticamente após sair.
- `-v <diretório local>:/app`: monta o diretório atual no container, permitindo acesso aos arquivos do projeto.

---

## 🛑 Parando o Container

Para finalizar o ambiente e sair do container interativo, você pode simplesmente pressionar:

```bash
CTRL + D
```

ou

```bash
exit
```

🔁 Como usamos a opção `--rm`, o container será automaticamente removido após sair, garantindo que o ambiente fique limpo.

---

## 📂 Dentro do Container

Uma vez dentro do container, navegue até a pasta `/app`, onde seus arquivos estão disponíveis:

```bash
cd /app
```

### 🔨 Compilando o código

Você pode compilar de duas formas:

1. Usando o Makefile (se estiver disponível):

```bash
make
```

2. Ou compilando diretamente com `gcc`:

```bash
gcc src/main.c -o scheduler
```

### ▶️ Executando o programa

Depois de compilar, basta rodar o executável:

```bash
./scheduler
```

---

## ✅ Requisitos

- Docker instalado (https://www.docker.com/)
- Código-fonte do projeto no diretório atual

---

## 💡 Dica

Se quiser modificar o código localmente e testar rapidamente, basta salvar as alterações e reexecutar o container. O volume `-v` garante que as mudanças estejam disponíveis no ambiente do container em tempo real.

---

## 📁 Estrutura esperada do projeto

```
.
├── Dockerfile
├── Makefile (opcional)
├── src/
│   └── main.c
└── README.md
```

---

## 📜 Licença

Distribuído sob a licença MIT. Veja `LICENSE` para mais informações.
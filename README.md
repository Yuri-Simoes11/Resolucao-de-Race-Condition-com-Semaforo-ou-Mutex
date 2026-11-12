# PSI-Microcontroladores2-Aula06
Atividade: Resolução de Race Condition com Semáforo ou Mutex

## 🎯 Objetivos da Atividade
Nesta atividade, os alunos deverão:
- Criar um código (linguagem livre) que apresenta **condições de corrida (race conditions)** (uso liberado de IA generativa).
- Trabalhar em **duplas**, com **avaliação cruzada interna** entre os integrantes do grupo (não deve usar IA generativa).
- Aplicar **testes estruturados** com pré-condição, etapas de teste e pós-condição (não deve usar IA generativa).
- Demonstrar como o problema de concorrência foi **identificado e resolvido** com uso de semáforo ou mutex (não deve usar IA generativa).

## 🧠 Etapas da Atividade

### **1️⃣ Criação do Código Anterior**
- Cada integrante do grupo deverá **criar um código**, e **executar o código do colega** que contém uma race condition.
- Documentar:
  - O comportamento incorreto observado.
  - O momento em que o erro ocorre (condição específica, sequência de eventos, etc.).

### **2️⃣ Planejamento de Testes**
Para cada cenário, descreva **três casos de teste** seguindo o formato abaixo:

| Caso de Teste | Pré-condição | Etapas de Teste | Pós-condição Esperada |
|----------------|---------------|------------------|------------------------|
| 1 | ... | ... | ... |
| 2 | ... | ... | ... |
| 3 | ... | ... | ... |

### **3️⃣ Correção e Reteste**
- Corrigir o código para **eliminar a race condition** usando semáforo ou mutex.
- Reexecutar **os mesmos casos de teste** e registrar:
  - As mudanças feitas, justificando sua escolha pelo mutex ou semáforo.
  - O resultado após a correção com evidências (capturas de tela por exemplo).

### **4️⃣ Avaliação Interna (entre colegas do mesmo grupo)**
Cada integrante deverá:
1. Criar o código de race condition.
2. Executar o código original do colega conforme os testes planejados.
3. Executar o código corrigido do colega conforme os testes planejados.  
4. Registrar uma **avaliação curta** (pode ser no final do README):
   - O que estava errado antes.  
   - O que mudou com a correção.
   - Se o comportamento agora é estável.  

## 📦 Entregáveis

No repositório da dupla, incluir:
1. `README.md` (este arquivo) contendo:
   - Nomes dos integrantes.
   - 2 Cenários escolhidos.
   - 6 Casos de teste.
   - Descrição das 2 race conditions e das soluções.
   - 2 Avaliações cruzadas.
2. Código-fonte organizado (considerando um código original e um corrigido por cada integrante, então 4 subpastas no total):
   - `codigo_original/`
   - `codigo_corrigido/`
3. Evidências (prints, logs, vídeos curtos, etc.) da execução dos testes.

---

**Entrega:** um PDF do markdown final (com o link do repositório aberto) no Moodle.

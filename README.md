# Atividade Race Conditions
Link github: https://github.com/Yuri-Simoes11/Resolucao-de-Race-Condition-com-Semaforo-ou-Mutex 

Integrantes do grupo: Henrique Isonaka, Pedro Domingues, Yuri Simões.

# Problemas observados e Testes
**Código 1**: O race condition é observado no processo de incrementação, onde ambas as threads tentam acessar um recurso compartilhado, mas acabam acessando o recurso antes de que a outra thread termine seu processo de incrementação, o que acaba gerando que ambas incrementem sobre o mesmo número, fazendo com que se perca uma incrementação a cada vez que o ciclo roda. Dessa forma, metade dos incrementos são perdidos.

- <ins> Teste 1: Código com valores originais- quatidade média de incrementos </ins>
  
Pré-condição: iniciar a execução do código no microcontrolador sem nenhuma modificação (10000 de incrementos totais, e contador com valor inicial 0)

Etapas de teste: observar o resultado obtido e comparar com o resultado esperado, que é 20000.

Pós-condição esperada: o valor obtido deve ser o mesmo do valor esperado, sendo que nenhum incremento deve ser perdido.

Pós condição esperada não foi obtida: O resultado obtido foi 10000,  e não 20000, sendo que assim foram perdidos 10000 incrementos.

<img width="902" height="675" alt="Captura de tela 2026-11-22 121940" src="https://github.com/user-attachments/assets/a7add7a6-78dd-49c1-9f62-b6e3cea7fe21" />


- <ins>Teste 2: Código com total de incrementos modificado - muitos incrementos </ins>

Pré-condição: iniciar a execução do código no microcontrolador modificando o total de incrementos (50000 de incrementos totais, e contador com valor inicial 0)

Etapas de teste: observar o resultado obtido e comparar com o resultado esperado, que é 100000.

Pós-condição esperada: o valor obtido deve ser o mesmo do valor esperado, sendo que nenhum incremento deve ser perdido.

Pós condição esperada não foi obtida: O resultado obtido foi 50000,  e não 100000, sendo que assim foram perdidos 50000 incrementos.

<img width="767" height="572" alt="Captura de tela 2026-11-22 122006" src="https://github.com/user-attachments/assets/e54d6d43-5684-4b3c-a869-464d16313e08" />

- <ins>Teste 3: Código com total de incrementos modificado - poucos incrementos </ins>

Pré-condição: iniciar a execução do código no microcontrolador modificando o total de incrementos e o valor inicial do contador (699 de incrementos totais, e contador com valor inicial 0)

Etapas de teste: observar o resultado obtido e comparar com o resultado esperado, que é 1398.

Pós-condição esperada: o valor obtido deve ser o mesmo do valor esperado, sendo que nenhum incremento deve ser perdido.

Pós condição esperada não foi obtida: O resultado obtido foi 699,  e não 1398, sendo que assim foram perdidos 699 incrementos.

<img width="673" height="506" alt="Captura de tela 2026-11-22 122217" src="https://github.com/user-attachments/assets/b8ebf137-12bb-4482-b911-b677f027e123" />

---
**Código 2**: O problema é observado no processo de modificar a variável global saldo_vitrine, que fica variando entre valores negativos e positivos, nunca representando o valor real do saldo da vitrine real. Isso acontece porque ocorre uma Race condition na variável saldo_vitrine em que uma thread modifica seu valor baseando-se no último valor obtido por ela no início de sua execução que é sempre o último dado por ela mesma. Dessa forma, é como se as duas threads trabalhassem com versões diferentes da mesma variável global, sendo que uma sempre decrementa em um seu valor e a outra sempre acrescenta um ao seu valor, não havendo relação nenhuma entre seu valores. Assim, esse erro ocorre constantemente no valor da variável global.

- <ins>Teste 1: Análise da variação da variavel global </ins>

Pré-condição: iniciar a execução do código no microcontrolador sem nenhuma modificação (saldo da vitrine 0, ou seja, sem nenhum pão na vitrine e buffer sem valores inicializados).

Etapas de teste: observar como a variável global saldo da vitrine se comporta enquanto o programa executa e se a alteração de seus valores é coerente, ou seja, os valores se alteram de 1 em 1 a cada linha do serial monitor.

Pós-condição esperada: Independentemente do tempo da execução, o saldo da vitrine deve sempre mudar seu valor de 1 em 1.

Pós-condição esperada não foi atingida: já nas primeiras linhas o saldo varia abruptamente de 1 para -1 (variação de 2).

<img width="902" height="674" alt="Captura de tela 2026-11-22 122421" src="https://github.com/user-attachments/assets/3722c8e5-83f9-4574-b5f5-7fbb6b5819f7" />

- <ins>Teste 2: Análise do valor da variável global </ins>


Pré-condição: iniciar a execução do código no microcontrolador sem nenhuma modificação (saldo da vitrine 0, ou seja, sem nenhum pão na vitrine e buffer sem valores inicializados).

Etapas de teste: observar se a variável global saldo da vitrine não atinge valores incoerentes durante a execução das threads. Além de ela não poder ser um valor negativo, ela também não pode exceder o seu limite, sendo esse de 10 pães.

Pós-condição esperada: Independente do tempo da execução, o saldo da vitrine nunca pode ser negativo ou maior do que 10.

Pós-condição esperada não foi atingida: o saldo da vitrine ficou negativo em toda linha gerada pela thread do cliente no monitor serial e também ultrapassou o limite da vitrine de 10 pães, chegando a até 12. Além disso, o código não foi capaz de executar infinitamente, pois deu um fatal error quando a thread do cliente ia terminar de executar seu looping pela 8 vez.

<img width="902" height="674" alt="Captura de tela 2026-11-22 122605" src="https://github.com/user-attachments/assets/9b850627-0fd0-45d4-94a6-857b21737ad7" />

- <ins>Teste 3: Análise dos pães no buffer da vitrine </ins>

Pré-condição: iniciar a execução do código no microcontrolador sem nenhuma modificação (saldo da vitrine 0, ou seja, sem nenhum pão na vitrine e buffer sem valores inicializados).

Etapas de teste: observar como as threads “interagem com os pães”, verificando se a thread do padeiro sempre cria pães novos e se o cliente sempre pega pães existentes na vitrine.

Pós-condição esperada: O padeiro nunca cria o mesmo pão duas vezes, sempre aumentando o número do novo pão criado em 1, e o cliente sempre pega o último pão adicionado na vitrine.

Pós-condição esperada não foi atingida: A partir do segundo looping de execução a thread do cliente parou de pegar pães existentes na vitrine e começou a pegar pães com um numeração inexistente, começando pelo pão de numeração 0 e depois pelos pães marcados por números muito grandes e aleatórios, provavelmente representativos de lixo da memória.

<img width="899" height="675" alt="Captura de tela 2026-11-22 122758" src="https://github.com/user-attachments/assets/30ff0a78-e8ff-4b21-a4f5-adba9bbabbc6" />

---
**Código 3** :A condição de deadlock é vista após um número aleatório de interações dado que há um pseudo-randomizador de escolhas (feito pela IA). Há 2 recursos que as duas threads solicitam. Quando uma thread pega um deles e a outra pega o outro, ocorre o deadlock. A mensagem “Sistema Funcionando” é impressa a cada 2 segundos para mostrar que o código ainda está sendo executado mas as threads estão em impasse.
É notado que o código é pseudo-aleatório, dando erro somente após uma quantidade específica de execuções. O erro ocorrerá quando ambas as threads solicitarem itens diferentes (impressora e scanner) e a outra thread não conseguir solicitar o segundo item a tempo, fazendo com que surja o deadlock.

- <ins>Teste 1: Análise do código nativo </ins>
  
Pré-condição: iniciar a execução do código no microcontrolador sem nenhuma modificação (prioridades iguais).

Etapas de teste: observar se o código ficará com ambas tarefas executando continuamente e constando que ambas tarefas são concluídas.

Pós-condição esperada: Independentemente do tempo da execução, as mensagens se repetirão continuamente até que o monitor serial seja fechado.

Pós-condição esperada não foi atingida: ocorre um momento que somente aparece a mensagem de sistema funcionando e as tarefas param de executar. A quantidade de iterações necessárias é aleatória.

<img width="901" height="505" alt="Captura de tela 2026-11-22 123250" src="https://github.com/user-attachments/assets/2023bb45-12eb-44f7-9482-610983aff811" />

- <ins>Teste 2: Alteração das prioridades</ins>

Pré-condição: iniciar a execução do código no microcontrolador alterando os valores de prioridades das threads.

Etapas de teste: checar se o código continuará executando continuamente, monitorando as saídas.

Pós-condição esperada: Independentemente do tempo da execução, as mensagens se repetirão continuamente até que o monitor serial seja fechado.

Pós-condição esperada não foi atingida: o erro aparece da mesma forma.

<img width="903" height="504" alt="Captura de tela 2026-11-22 124135" src="https://github.com/user-attachments/assets/841fa56b-9418-44b1-9dd3-c980bb27463a" />


- <ins>Teste 3: Quantidade de operações iniciais</ins>

Pré-condição: modificar o valor de operações iniciais de forma extrema (valores >20 se possível).

Etapas de teste: observar se o código ficará com ambas tarefas executando continuamente e constando que ambas tarefas são concluídas. Espera-se que haja maior quantidade de iterações.

Pós-condição esperada: o código realize mais execuções que o código anterior.

Pós-condição esperada foi atingida: O código irá executar consistentemente um número maior de vezes, porém o erro ainda aparecerá posteriormente.

<img width="899" height="507" alt="Captura de tela 2026-11-22 131821" src="https://github.com/user-attachments/assets/f010ca34-00f6-4d58-9a7d-b7ae67d3d603" />

# Correções e Testes

**Código 1**: A correção feita consistiu em adicionar um mutex no código, fazer ambas as  threads terem que adquirir o mutex antes  de iniciarem a função de soma (somar_com_falha)  e liberar o mutex somente após terminarem essa função de soma. Dessa forma, uma thread não consegue iniciar a função de soma enquanto a outra está executando-a mesmo com o k_yield, que cede a execução para outra thread de mesma prioridade, estando presente nessa função, já que o mutex só é cedido para essa thread após o término da função de soma. Em outras palavras, esse mutex fez com que ficasse impossível de as duas threads executarem a função de soma ao mesmo tempo, o que era o fator que gerava a race condition. Assim, o comportamento do código ficou estável, funcionando corretamente de forma consistente.

Motivo pela escolha do mutex: era necessário que só uma thread executasse por vez a função soma para evitar a race condition, o que fazia dessa parte do código uma região crítica. Assim, o mutex se apresentou como uma solução mais correta do que o semáforo.

Obs: Só se percebeu esse fato depois, mas o semáforo com 1 de limite e 1 de valor inicial, que era a primeira ideia de solução, deveria ter sido substituído por um mutex, já que só uma thread executa a função soma por vez. Dessa forma, o código foi atualizado uma vez para substituir o antigo semáforo pelo mutex, mesmo ambos fornecendo os mesmos resultados.

- <ins>Teste 1: Código com valores originais- quatidade média de incrementos</ins>

Pré-condição: iniciar a execução do código no microcontrolador sem nenhuma modificação (10000 de incrementos totais, e contador com valor inicial 0)

Etapas de teste: observar o resultado obtido e comparar com o resultado esperado, que é 20000.

Pós-condição esperada: o valor obtido deve ser o mesmo do valor esperado, sendo que nenhum incremento deve ser perdido.

Pós condição esperada foi obtida: O resultado obtido foi 20000, como esperado.

<img width="678" height="506" alt="Captura de tela 2026-11-22 132240" src="https://github.com/user-attachments/assets/7fd4fd66-f3a4-4e10-b7a9-4a0fcbe90042" />

- <ins>Teste 2: Código com total de incrementos modificado - muitos incrementos</ins>

Pré-condição: iniciar a execução do código no microcontrolador modificando o total de incrementos (50000 de incrementos totais, e contador com valor inicial 0)

Etapas de teste: observar o resultado obtido e comparar com o resultado esperado, que é 100000.

Pós-condição esperada: o valor obtido deve ser o mesmo do valor esperado, sendo que nenhum incremento deve ser perdido.

Pós condição esperada foi obtida: O resultado obtido foi 100000, como esperado.

<img width="668" height="498" alt="Captura de tela 2026-11-22 132403" src="https://github.com/user-attachments/assets/c63ea524-44a8-4306-b2a3-f964e635a273" />

- <ins>Teste 3: Código com total de incrementos modificado - poucos incrementos</ins>

Pré-condição: iniciar a execução do código no microcontrolador modificando o total de incrementos e o valor inicial do contador (699 de incrementos totais, e contador com valor inicial 0)

Etapas de teste: observar o resultado obtido e comparar com o resultado esperado, que é 1398.

Pós-condição esperada: o valor obtido deve ser o mesmo do valor esperado, sendo que nenhum incremento deve ser perdido.

Pós condição esperada foi obtida: O resultado obtido foi 1398, como esperado.

<img width="648" height="483" alt="Captura de tela 2026-11-22 133012" src="https://github.com/user-attachments/assets/3158d6f4-a4a1-450d-be28-8efe11d70715" />

---

**Código 2**: Correção feita pela adição de semáforos, dado que a modificação de variáveis não leva ao desaparecimento do erro de race condition. Usa-se três semáforos para garantir a execução correta, sendo um para verificar a existência de pães, outro se há espaço na vitrine, e outro para checar se é possível usar a thread (para que não ocorra preempção). O mutex sem_vagas evita que o padeiro “faça pães sem necessidade”, o sem_paes evita que o cliente tente pegar um pão não existente, e o sem_mutex garante que não ocorra preempção. O sistema continua funcionando pelo LIFO (Last In First Out). O comportamento é estável e conforme o planejado.

- <ins>Teste 1:  Análise da variação da variavel global </ins>

Pós-condição esperada: Independentemente do tempo da execução, o saldo da vitrine deve sempre mudar seu valor de 1 em 1.

Pós-condição esperada atingida: houve o incremento unitário da variável.

<img width="898" height="503" alt="Captura de tela 2026-11-22 133439" src="https://github.com/user-attachments/assets/06154d90-007b-41d5-ae53-c084c2fcfcaa" />

- <ins>Teste 2:  Análise do valor da variável global </ins>

Pós-condição esperada: Independente do tempo da execução, o saldo da vitrine nunca pode ser negativo ou maior do que 10.

Pós-condição esperada atingida: para forçar o resultado mais facilmente, o msleep do padeiro foi reduzido para 200 e foi adicionado um msleep de 500ms após o cliente liberar as threads. Com isso, é possível notar que a variável nunca passa de 10. A condição negativa deixa de existir ao todo.

<img width="899" height="503" alt="Captura de tela 2026-11-22 133546" src="https://github.com/user-attachments/assets/122553af-b7ec-4995-a922-c8fa08fedfc0" />

- <ins>Teste 3:Análise dos pães no buffer da vitrine </ins>

Pós-condição esperada: O padeiro nunca cria o mesmo pão duas vezes, sempre aumentando o número do novo pão criado em 1, e o cliente sempre pega o último pão adicionado na vitrine.

Pós-condição esperada foi atingida: (ainda com algumas variáveis modificadas para criar uma condição propensa) é possível notar na foto abaixo como a regra do LIFO (Last In First Out) continua sendo aplicada e o padeiro não cria repetidamente o mesmo pão.

<img width="899" height="499" alt="Captura de tela 2026-11-22 133712" src="https://github.com/user-attachments/assets/540f5010-4c6e-4eb8-bb10-228944d319a3" />

---

**Código 3**: Para realizar a correção, foi removida a inversão aleatória da ordem de aquisição dos mutexes (inverter) e padronizada uma ordem única de bloqueio em todas as threads (sempre adquirir mutex_impressora primeiro e mutex_scanner depois), liberando na ordem inversa (LIFO). Isso elimina a espera circular que causava o deadlock no Zephyr RTOS. Agora o comportamento do código se demonstra como estável, não apresentando erros independentemente do número de iterações.

- <ins>Teste 1: Análise do código nativo</ins>

Pré-condição: iniciar a execução do código no microcontrolador sem nenhuma modificação (prioridades iguais).

Etapas de teste: observar se o código ficará com ambas tarefas executando continuamente e constando que ambas tarefas são concluídas.

Pós-condição esperada: Independentemente do tempo da execução, as mensagens se repetirão continuamente até que o monitor serial seja fechado.

Pós-condição esperada foi atingida: As mensagens se repetem continuamente até que o monitor serial seja fechado. As tarefas não param de executar em nenhum momento.

<img width="692" height="515" alt="Captura de tela 2026-11-22 133942" src="https://github.com/user-attachments/assets/01daaedd-4ae7-4159-bee3-937bb08a56a7" />

- <ins>Teste 2: Alteração das prioridades</ins>

Pré-condição: iniciar a execução do código no microcontrolador alterando os valores de prioridades das threads (thread da impressão com prioridade maior do que a thread da digitalização).

Etapas de teste: checar se o código continuará executando continuamente, monitorando as saídas.

Pós-condição esperada: Independentemente do tempo da execução, as mensagens se repetirão continuamente até que o monitor serial seja fechado.

Pós-condição esperada foi atingida: As mensagens se repetem continuamente assim, como no teste 1, o que indica que as tarefas não param de executar em nenhum momento.

<img width="653" height="488" alt="Captura de tela 2026-11-22 134048" src="https://github.com/user-attachments/assets/be4b8ebd-ab1e-47ac-8a44-87c91a4b617c" />

- <ins>Teste 3: Quantidade de operações iniciais</ins>

Pré-condição: modificar o valor de operações iniciais de forma extrema (valores >20 se possível).

Etapas de teste: observar se o código ficará com ambas tarefas executando continuamente e constando que ambas tarefas são concluídas.

Pós-condição esperada: assim como nos testes anteriores, independentemente do tempo da execução, as mensagens se repetirão continuamente até que o monitor serial seja fechado.

Pós-condição esperada foi atingida: As mensagens se repetem continuamente assim, como nos testes 1 e 2, o que indica que as tarefas não param de executar em nenhum momento. Dessa forma é possível constatar que essa alteração não afetou o funcionamento do código.

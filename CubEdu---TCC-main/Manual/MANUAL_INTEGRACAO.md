# Manual de Integração das Placas do CubEdu

## 1. Objetivo

Este manual descreve o procedimento de integração eletrônica e mecânica das placas do CubEdu, apresentando a sequência correta de montagem dos subsistemas e os cuidados necessários para garantir a integridade dos conectores e do barramento de comunicação.

<p align="center">
  <img
    width="613"
    height="831"
    alt="Vista explodida do empilhamento do CubEdu"
    src="https://github.com/user-attachments/assets/3bc9e90f-864c-4f88-96b9-ab225afda27f"
  />
</p>

<p align="center">
  <em>Figura 1 – Vista explodida do empilhamento dos subsistemas do CubEdu.</em>
</p>


---

## 2. Arquitetura do Empilhamento

O CubEdu utiliza uma arquitetura modular baseada em placas empilhadas (stackable boards), interligadas por conectores verticais que distribuem alimentação e sinais entre os subsistemas.

A integração deve seguir obrigatoriamente a seguinte ordem:

| Ordem | Subsistema | Função |
|---------|---------|---------|
| 1 | EPS | Sistema de Potência e Distribuição de Energia |
| 2 | OBC | Computador de Bordo |
| 3 | ADC | Aquisição e Condicionamento de Dados |
| 4 | Payload | Sensores e Experimentos da Missão |

A configuração final do empilhamento deve seguir a disposição abaixo:

```text
┌─────────────────────┐
│       Payload       │
├─────────────────────┤
│         ADC         │
├─────────────────────┤
│         OBC         │
├─────────────────────┤
│         EPS         │
└─────────────────────┘
         BASE
```

---

## 3. Alinhamento das Placas

> **ATENÇÃO:** Todas as placas possuem uma seta de referência gravada na PCB indicando sua orientação de montagem.

Durante a integração, as setas de todas as placas devem permanecer apontando para a mesma direção.

O alinhamento correto é fundamental para garantir a correspondência entre os sinais do barramento, alimentação elétrica e linhas de comunicação entre os módulos.

```text
      ↑
  Payload
      ↑
     ADC
      ↑
     OBC
      ↑
     EPS
```

Caso uma placa seja instalada invertida, os conectores poderão encaixar mecanicamente, porém os sinais elétricos ficarão desalinhados, resultando em falhas de funcionamento e podendo causar danos permanentes aos componentes.

Antes da instalação de cada módulo, verifique visualmente o alinhamento da seta de referência.

---

## 4. Integração da Placa EPS

A placa EPS (Electrical Power System) constitui a base do empilhamento e deve ser instalada primeiro.

### Procedimento

1. Posicionar a placa EPS em uma base plana.
2. Inserir as células de bateria em seus alojamentos.
3. Verificar a conexão do carregador e dos reguladores de tensão.
4. Confirmar a fixação dos conectores do barramento vertical.

### Verificações

- Polaridade correta das baterias.
- Ausência de curto-circuito.
- Fixação mecânica adequada.
- Seta de referência orientada corretamente.

---

## 5. Integração da Placa OBC

Após a instalação da EPS, realizar a montagem da OBC (On-Board Computer).

### Procedimento

1. Instalar os espaçadores sobre a EPS.
2. Alinhar os conectores da OBC com o barramento.
3. Verificar o alinhamento da seta de orientação.
4. Encaixar cuidadosamente a placa.

### Verificações

- Encaixe completo dos conectores.
- Ausência de deformação da placa.
- Espaçadores corretamente posicionados.
- Alinhamento das setas mantido.

---

## 6. Integração da Placa ADC

A placa ADS (Attitude Determination System) deve ser instalada sobre a OBC.

### Procedimento

1. Instalar os espaçadores intermediários.
2. Alinhar os conectores com o barramento vertical.
3. Confirmar a orientação da seta de referência.
4. Inserir a placa uniformemente até o encaixe completo.

### Verificações

- Conectores totalmente encaixados.
- Ausência de folgas excessivas.
- Alinhamento entre barramentos.
- Orientação correta da placa.

---

## 7. Integração da Placa Payload

A placa Payload é o último módulo a ser instalado.

### Procedimento

1. Posicionar os espaçadores superiores.
2. Alinhar a placa com os conectores do barramento.
3. Verificar a orientação da seta.
4. Realizar o encaixe completo.

### Verificações

- Fixação mecânica adequada.
- Sensores corretamente posicionados.
- Conectores totalmente acoplados.
- Ausência de interferências mecânicas.

---

## 8. Inspeção Final

Após a integração completa do empilhamento, deve-se realizar uma inspeção visual e elétrica do conjunto.

### Checklist

- [ ] EPS instalada corretamente.
- [ ] OBC instalada corretamente.
- [ ] ADC instalada corretamente.
- [ ] Payload instalada corretamente.
- [ ] Todas as setas estão alinhadas.
- [ ] Todos os conectores estão totalmente encaixados.
- [ ] Não existem interferências mecânicas entre os módulos.
- [ ] Continuidade elétrica verificada.
- [ ] Sistema inspecionado antes da energização.

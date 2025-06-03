# Monitoramento da Acidificação das Águas Litorâneas 
Desenvolvimento de uma aplicação baseada em Arduino para o monitoramento da qualidade das águas oceânicas, utilizando sensores de pH. O sistema proposto integra sensores de pH, capazes de fornecer dados em tempo real sobre a acidez da água, e um microcontrolador ESP-32, que processa as informações coletadas.
Os dados são integrados via Node-Red para um bucket InfluxDb, a partir disso é possível criar painéis gerenciais por exemplo utilizando o grafana.

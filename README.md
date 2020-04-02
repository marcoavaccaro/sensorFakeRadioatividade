# sensorFakeRadioatividade

![](/images/chernobyl.jpg)

Código fonte para ser usado no ESP32.

O objetivo deste sistema fake é simular a leitura de elementos radioativos sem precisar se expor a um.

É utilizado beacons BLE (bluetooth low energy) para simular emissões radioativas e um leitor BLE para detectar estes itens.

Como detector é utilizado um ESP32. Este microcontrolador possuí BLE embutido, permitindo que façamos as leituras de Beacons próximos.

![](/images/ESP32.jpg)

O código é simples, quanto mais próximo está o elemento radioativo com maior potência o detector irá receber as leituras. Um led irá aumentar a frequência de ON/OFF indicando a próximidade (quanto mais próximo mais rapidamente o LED irá piscar). Também é exibido a distância aproximada do elemento radioativo em uma tela.


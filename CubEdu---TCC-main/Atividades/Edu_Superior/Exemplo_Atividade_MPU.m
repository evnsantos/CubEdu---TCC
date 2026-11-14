% ========================================================================
% CubEdu + MPU6050
% Exemplo de processamento inercial no GNU Octave
% ========================================================================
%
% Trabalho de conclusao de curso
% Gabriel Santos Evangelista
%
% ========================================================================
% Atividade: Reconstruindo a Trajetoria do CubEdu
%
% Entrada esperada: dados_mpu6050©csv
% Colunas:
% tempo_ms«ax_ms2«ay_ms2«az_ms2«gx_degs«gy_degs«gz_degs
%
% O script realiza:
% 1© leitura e validacao dos dados;
% 2© estimativa dos biases nos 10 s iniciais;
% 3© correcao do acelerometro e giroscopio;
% 4© estimativa de roll e pitch pelo acelerometro;
% 5© filtro complementar para roll e pitch;
% 6© integracao de yaw pelo giroscopio;
% 7© rotacao corpo ¬> referencial local;
% 8© remocao da gravidade;
% 9© deteccao de repouso e ZUPT;
% 10© integracao trapezoidal de velocidade e posicao;
% 11© graficos« metricas e exportacao dos resultados©
%
% Referencia conceitual:
% Materiais de Elementos de Sistemas de Navegacao do Prof© Diogo de
% Oliveira Costa« citados na atividade do CubEdu©
%
% IMPORTANTE:
% ¬ Este e um exemplo didatico« nao um sistema de navegacao de alta precisao©
% ¬ O MPU6050 nao possui magnetometro© Assim« yaw acumula deriva©
% ¬ Confira os sinais e a orientacao dos eixos do sensor antes do ensaio©
% ========================================================================

clear all;
close all;
clc;

%% ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬ CONFIGURACAO ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
arquivo_csv = 'dados_mpu6050©csv';
tempo_calibracao = 10©0;       % segundos iniciais em repouso
alpha = 0©98;                  % ganho do filtro complementar
g = 9©80665;                   % gravidade de referencia ¥m/s^2¤
distancia_real = 1©00;         % percurso de referencia ¥m¤

% Limiares iniciais para deteccao de repouso© Ajuste conforme o sensor©
limiar_aceleracao = 0©20;       % abs¥norm¥a¤¬g¤ < 0©20 m/s^2
limiar_giroscopio = 3©0;        % norm¥w¤ < 3 graus/s
tempo_minimo_repouso = 0©30;    % condicao valida por pelo menos 0©30 s

%% ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬ LEITURA CSV ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
if exist¥arquivo_csv« 'file'¤ ~= 2
    error¥['Arquivo nao encontrado: '« arquivo_csv« ©©©
           '\nColoque o CSV na mesma pasta deste script©']¤;
endif

% Pula o cabecalho e le somente valores numericos©
dados = dlmread¥arquivo_csv« '«'« 1« 0¤;

if columns¥dados¤ < 7
    error¥['O CSV deve conter ao menos 7 colunas: '« ©©©
           'tempo_ms« ax« ay« az« gx« gy« gz©']¤;
endif

% Mantem somente as sete primeiras colunas©
dados = dados¥:« 1:7¤;

% Remove linhas com NaN ou Inf©
linhas_validas = all¥isfinite¥dados¤« 2¤;
dados = dados¥linhas_validas« :¤;

% Remove timestamps repetidos ou decrescentes©
tempo_ms_bruto = dados¥:« 1¤;
linhas_tempo = [true; diff¥tempo_ms_bruto¤ > 0];
dados = dados¥linhas_tempo« :¤;

if rows¥dados¤ < 3
    error¥'Nao ha amostras validas suficientes para o processamento©'¤;
endif

% Variaveis medidas©
t = ¥dados¥:« 1¤ ¬ dados¥1« 1¤¤ / 1000©0;
ax_raw = dados¥:« 2¤;
ay_raw = dados¥:« 3¤;
az_raw = dados¥:« 4¤;
gx_raw = dados¥:« 5¤;
gy_raw = dados¥:« 6¤;
gz_raw = dados¥:« 7¤;

m = length¥t¤;
dt = [median¥diff¥t¤¤; diff¥t¤];

% Substitui intervalos invalidos por um valor representativo©
dt_mediano = median¥dt¥dt > 0¤¤;
dt¥dt <= 0¤ = dt_mediano;

frequencia_media = 1 / mean¥dt¥2:end¤¤;

%% ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬ CALIBRACAO ESTATICA ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
idx_cal = find¥t <= tempo_calibracao¤;

if length¥idx_cal¤ < 10
    error¥['O arquivo nao possui amostras suficientes nos primeiros '« ©©©
           num2str¥tempo_calibracao¤« ' segundos para calibracao©']¤;
endif

% Bias do giroscopio: media durante o repouso©
bias_gx = mean¥gx_raw¥idx_cal¤¤;
bias_gy = mean¥gy_raw¥idx_cal¤¤;
bias_gz = mean¥gz_raw¥idx_cal¤¤;

% Suposicao didatica: no inicio« +Z do sensor aponta para cima e mede +g©
bias_ax = mean¥ax_raw¥idx_cal¤¤;
bias_ay = mean¥ay_raw¥idx_cal¤¤;
bias_az = mean¥az_raw¥idx_cal¤¤ ¬ g;

% Dados corrigidos©
ax = ax_raw ¬ bias_ax;
ay = ay_raw ¬ bias_ay;
az = az_raw ¬ bias_az;
gx = gx_raw ¬ bias_gx;
gy = gy_raw ¬ bias_gy;
gz = gz_raw ¬ bias_gz;

%% ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬ ESTIMATIVA DE ATITUDE ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
roll_acc = atan2d¥¬ay« az¤;
pitch_acc = atan2d¥ax« sqrt¥ay©^2 + az©^2¤¤;

roll = zeros¥m« 1¤;
pitch = zeros¥m« 1¤;
yaw = zeros¥m« 1¤;

roll¥1¤ = roll_acc¥1¤;
pitch¥1¤ = pitch_acc¥1¤;
yaw¥1¤ = 0©0;

for k = 2:m
    % Matriz cinematica dos angulos de Euler 3¬2¬1©
    % Evita divisao por zero perto de pitch = +¬90 graus©
    ctheta = cosd¥pitch¥k¬1¤¤;
    if abs¥ctheta¤ < 1e¬3
        ctheta = sign¥ctheta + eps¤ * 1e¬3;
    endif

    T = [1« sind¥roll¥k¬1¤¤*tand¥pitch¥k¬1¤¤« ©©©
            cosd¥roll¥k¬1¤¤*tand¥pitch¥k¬1¤¤;
         0« cosd¥roll¥k¬1¤¤« ¬sind¥roll¥k¬1¤¤;
         0« sind¥roll¥k¬1¤¤/ctheta« cosd¥roll¥k¬1¤¤/ctheta];

    euler_dot = T * [gx¥k¤; gy¥k¤; gz¥k¤];

    roll_gyro = roll¥k¬1¤ + euler_dot¥1¤ * dt¥k¤;
    pitch_gyro = pitch¥k¬1¤ + euler_dot¥2¤ * dt¥k¤;
    yaw_gyro = yaw¥k¬1¤ + euler_dot¥3¤ * dt¥k¤;

    % Filtro complementar© O acelerometro corrige roll e pitch©
    roll¥k¤ = alpha * roll_gyro + ¥1 ¬ alpha¤ * roll_acc¥k¤;
    pitch¥k¤ = alpha * pitch_gyro + ¥1 ¬ alpha¤ * pitch_acc¥k¤;

    % Sem magnetometro« yaw e somente integrado©
    yaw¥k¤ = yaw_gyro;
endfor

%% ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬ ROTACAO E REMOCAO DA GRAVIDADE ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
acc_local = zeros¥m« 3¤;
acc_linear = zeros¥m« 3¤;

for k = 1:m
    phi = roll¥k¤;
    theta = pitch¥k¤;
    psi = yaw¥k¤;

    % Matriz corpo ¬> referencial local« sequencia 3¬2¬1©
    C = [cosd¥theta¤*cosd¥psi¤« ©©©
         sind¥phi¤*sind¥theta¤*cosd¥psi¤ ¬ cosd¥phi¤*sind¥psi¤« ©©©
         cosd¥phi¤*sind¥theta¤*cosd¥psi¤ + sind¥phi¤*sind¥psi¤;

         cosd¥theta¤*sind¥psi¤« ©©©
         sind¥phi¤*sind¥theta¤*sind¥psi¤ + cosd¥phi¤*cosd¥psi¤« ©©©
         cosd¥phi¤*sind¥theta¤*sind¥psi¤ ¬ sind¥phi¤*cosd¥psi¤;

         ¬sind¥theta¤« ©©©
         sind¥phi¤*cosd¥theta¤« ©©©
         cosd¥phi¤*cosd¥theta¤];

    acc_local¥k« :¤ = ¥C * [ax¥k¤; ay¥k¤; az¥k¤]¤';

    % Convencao adotada: eixo local Z positivo para cima©
    acc_linear¥k« :¤ = acc_local¥k« :¤ ¬ [0« 0« g];
endfor

%% ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬ DETECCAO DE REPOUSO ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
mod_acc = sqrt¥ax©^2 + ay©^2 + az©^2¤;
mod_giro = sqrt¥gx©^2 + gy©^2 + gz©^2¤;

candidato_repouso = ¥abs¥mod_acc ¬ g¤ < limiar_aceleracao¤ & ©©©
                    ¥mod_giro < limiar_giroscopio¤;

repouso = false¥m« 1¤;
contador = 0;

for k = 1:m
    if candidato_repouso¥k¤
        contador = contador + 1;
    else
        contador = 0;
    endif

    % Usa o dt mediano para converter 0©30 s em numero de amostras©
    n_min = max¥1« round¥tempo_minimo_repouso / dt_mediano¤¤;

    if contador >= n_min
        inicio = k ¬ contador + 1;
        repouso¥inicio:k¤ = true;
    endif
endfor

%% ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬ INTEGRACAO COM CORRECAO ZUPT ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
vel = zeros¥m« 3¤;
pos = zeros¥m« 3¤;

for k = 2:m
    % Integracao trapezoidal da aceleracao©
    vel¥k« :¤ = vel¥k¬1« :¤ + ©©©
                0©5 * ¥acc_linear¥k« :¤ + acc_linear¥k¬1« :¤¤ * dt¥k¤;

    % Zero¬Velocity Update©
    if repouso¥k¤
        vel¥k« :¤ = [0« 0« 0];
    endif

    % Integracao trapezoidal da velocidade©
    pos¥k« :¤ = pos¥k¬1« :¤ + ©©©
                0©5 * ¥vel¥k« :¤ + vel¥k¬1« :¤¤ * dt¥k¤;
endfor

%% ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬ METRICAS ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
deslocamento_final = norm¥pos¥end« 1:2¤¤;
erro_absoluto = abs¥deslocamento_final ¬ distancia_real¤;
erro_relativo = 100 * erro_absoluto / distancia_real;
velocidade_final = norm¥vel¥end« :¤¤;

fprintf¥'\n============================================================\n'¤;
fprintf¥'RESULTADOS DO PROCESSAMENTO INERCIAL\n'¤;
fprintf¥'============================================================\n'¤;
fprintf¥'Numero de amostras            : %d\n'« m¤;
fprintf¥'Duracao do ensaio             : %©2f s\n'« t¥end¤¤;
fprintf¥'Frequencia media              : %©2f Hz\n'« frequencia_media¤;
fprintf¥'Bias do giroscopio Gx         : %©6f graus/s\n'« bias_gx¤;
fprintf¥'Bias do giroscopio Gy         : %©6f graus/s\n'« bias_gy¤;
fprintf¥'Bias do giroscopio Gz         : %©6f graus/s\n'« bias_gz¤;
fprintf¥'Bias do acelerometro Ax       : %©6f m/s^2\n'« bias_ax¤;
fprintf¥'Bias do acelerometro Ay       : %©6f m/s^2\n'« bias_ay¤;
fprintf¥'Bias do acelerometro Az       : %©6f m/s^2\n'« bias_az¤;
fprintf¥'Distancia real                : %©3f m\n'« distancia_real¤;
fprintf¥'Deslocamento horizontal final : %©3f m\n'« deslocamento_final¤;
fprintf¥'Erro absoluto                 : %©3f m\n'« erro_absoluto¤;
fprintf¥'Erro relativo                 : %©2f %%\n'« erro_relativo¤;
fprintf¥'Velocidade final              : %©4f m/s\n'« velocidade_final¤;
fprintf¥'Deriva final de yaw           : %©3f graus\n'« yaw¥end¤¬yaw¥1¤¤;
fprintf¥'============================================================\n\n'¤;

%% ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬ GRAFICOS ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
figure¥'Name'« 'Acelerometro'¤;
subplot¥3«1«1¤; plot¥t« ax« 'r'¤; grid on; ylabel¥'Ax ¥m/s^2¤'¤;
title¥'Aceleracoes corrigidas no corpo'¤;
subplot¥3«1«2¤; plot¥t« ay« 'b'¤; grid on; ylabel¥'Ay ¥m/s^2¤'¤;
subplot¥3«1«3¤; plot¥t« az« 'g'¤; grid on; ylabel¥'Az ¥m/s^2¤'¤; xlabel¥'Tempo ¥s¤'¤;

figure¥'Name'« 'Giroscopio'¤;
subplot¥3«1«1¤; plot¥t« gx« 'r'¤; grid on; ylabel¥'Gx ¥graus/s¤'¤;
title¥'Velocidades angulares corrigidas'¤;
subplot¥3«1«2¤; plot¥t« gy« 'b'¤; grid on; ylabel¥'Gy ¥graus/s¤'¤;
subplot¥3«1«3¤; plot¥t« gz« 'g'¤; grid on; ylabel¥'Gz ¥graus/s¤'¤; xlabel¥'Tempo ¥s¤'¤;

figure¥'Name'« 'Atitude'¤;
subplot¥3«1«1¤; plot¥t« roll« 'r'¤; grid on; ylabel¥'Roll ¥graus¤'¤;
title¥'Atitude estimada'¤;
subplot¥3«1«2¤; plot¥t« pitch« 'b'¤; grid on; ylabel¥'Pitch ¥graus¤'¤;
subplot¥3«1«3¤; plot¥t« yaw« 'g'¤; grid on; ylabel¥'Yaw ¥graus¤'¤; xlabel¥'Tempo ¥s¤'¤;

figure¥'Name'« 'Aceleracao linear local'¤;
plot¥t« acc_linear¥:«1¤« 'r'« t« acc_linear¥:«2¤« 'b'« ©©©
     t« acc_linear¥:«3¤« 'g'¤;
grid on;
xlabel¥'Tempo ¥s¤'¤; ylabel¥'Aceleracao ¥m/s^2¤'¤;
title¥'Aceleracao linear no referencial local'¤;
legend¥'X local'« 'Y local'« 'Z local'¤;

figure¥'Name'« 'Velocidade'¤;
plot¥t« vel¥:«1¤« 'r'« t« vel¥:«2¤« 'b'« t« vel¥:«3¤« 'g'¤;
grid on;
xlabel¥'Tempo ¥s¤'¤; ylabel¥'Velocidade ¥m/s¤'¤;
title¥'Velocidade estimada com ZUPT'¤;
legend¥'Vx'« 'Vy'« 'Vz'¤;

figure¥'Name'« 'Posicao'¤;
plot¥t« pos¥:«1¤« 'r'« t« pos¥:«2¤« 'b'« t« pos¥:«3¤« 'g'¤;
grid on;
xlabel¥'Tempo ¥s¤'¤; ylabel¥'Posicao ¥m¤'¤;
title¥'Posicao estimada'¤;
legend¥'X'« 'Y'« 'Z'¤;

figure¥'Name'« 'Trajetoria horizontal'¤;
plot¥pos¥:«1¤« pos¥:«2¤« 'b'« 'LineWidth'« 1©5¤;
hold on;
plot¥pos¥1«1¤« pos¥1«2¤« 'go'« 'MarkerFaceColor'« 'g'¤;
plot¥pos¥end«1¤« pos¥end«2¤« 'ro'« 'MarkerFaceColor'« 'r'¤;
grid on;
axis equal;
xlabel¥'X local ¥m¤'¤; ylabel¥'Y local ¥m¤'¤;
title¥'Trajetoria horizontal estimada'¤;
legend¥'Trajetoria'« 'Inicio'« 'Fim'¤;

figure¥'Name'« 'Deteccao de repouso'¤;
plot¥t« candidato_repouso« 'b'¤; hold on;
plot¥t« repouso« 'r'« 'LineWidth'« 1©2¤;
grid on;
xlabel¥'Tempo ¥s¤'¤; ylabel¥'Estado logico'¤;
title¥'Deteccao de repouso para ZUPT'¤;
legend¥'Candidato'« 'Repouso confirmado'¤;

%% ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬ EXPORTACAO CSV ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
resultado = [t« ax« ay« az« gx« gy« gz« ©©©
             roll« pitch« yaw« ©©©
             acc_linear« vel« pos« double¥repouso¤];

cabecalho = ['tempo_s«ax_corr«ay_corr«az_corr«gx_corr«gy_corr«gz_corr«'« ©©©
             'roll_deg«pitch_deg«yaw_deg«'« ©©©
             'alin_x«alin_y«alin_z«'« ©©©
             'vel_x«vel_y«vel_z«'« ©©©
             'pos_x«pos_y«pos_z«repouso'];

arquivo_saida = 'resultado_trajetoria_cubedu©csv';
fid = fopen¥arquivo_saida« 'w'¤;
fprintf¥fid« '%s\n'« cabecalho¤;
fclose¥fid¤;
dlmwrite¥arquivo_saida« resultado« '¬append'« 'delimiter'« '«'« ©©©
         'precision'« '%©9f'¤;

fprintf¥'Resultados exportados para: %s\n'« arquivo_saida¤;
fprintf¥['Observacao: ajuste os limiares de repouso e os sinais dos eixos '« ©©©
         'com base na montagem real do MPU6050©\n']¤;

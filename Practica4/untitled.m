clc; clear; close all

% Cargar datos desde el archivo
A = load('practica4.txt');
t = A(:,1);    % tiempo
u = A(:,2);    % entrada (escalón)
y = A(:,3);    % salida (respuesta del motor)

% Graficar señales originales
figure
plot(t, y, 'b', 'LineWidth', 1.2);hold on
plot(t, u, 'r--', 'LineWidth', 1.5)
ylim([0 6]);
xlabel('Tiempo (s)')
ylabel('Amplitud')
legend('Salida y(t)', 'Entrada u(t)')
title('Respuesta al escalón del motor')
grid on

%% Estimar inicio del escalón
du = diff(u);
[~, idx_start] = max(abs(du));  % Cambio más abrupto
t0 = t(idx_start);              % Tiempo de inicio del escalón

%% Modelo con t0 incorporado
% Modelo: y(t) = K*(1 - exp(-(t - t0)/tau)) * (t >= t0)
modelo = @(params, t) params(1)*(1 - exp(-(t - params(3))/params(2))) .* (t >= params(3));

% Estimación inicial: [K, tau, t0]
params0 = [max(y), 1, t0];

% Ajustar modelo a todos los datos
params = lsqcurvefit(modelo, params0, t, y);

% Extraer parámetros estimados
K   = params(1);
tau = params(2);
t0f = params(3);

fprintf('\nModelo estimado: G(s) = %.3f / (%.3f s + 1)\n', K, tau);
fprintf('\ninicia en t0 = %.3f s\n',t0f);
% Respuesta estimada
y_modelo = modelo(params, t);

% Graficar comparación
figure
plot(t, y, 'b', 'LineWidth', 1.5); hold on
plot(t, y_modelo, 'r--', 'LineWidth', 1.5)
xline(t0f, 'k:', 'Inicio estimado del escalón');
xlabel('Tiempo (s)')
ylabel('Salida')
legend('Respuesta real', 'Modelo estimado', 'Inicio del escalón')
title('Comparación entre respuesta real y modelo estimado')
grid on


Gs = tf(K, [tau 1]);

C0 = pidstd(1,1,1); 
C = pidtune(Gs, C0)
% %C = pidtune(Gs,'PIDF',1.0)
% C = pidtune(Gs, 'PID')
% 
% Kp = C.Kp
% Ki = C.Ki
% Kd = C.Kd
% 
% Ti = Kp / Ki;
% Td = Kd / Kp;
% 
% fprintf('Kp = %.3f\nTi = %.3f\nTd = %.3f\n', Kp, Ti, Td);
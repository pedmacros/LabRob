% Funci�n para mostrar telemetr�a de robot m�vil
% Laboratorio de Rob�tica 4� GIERM 18-19
% Federico Cuesta
% USO: Pasar como par�metro el nombre del archivo de telemetr�a con el
% sigiente contenido por fila:
% -Incremento de tiempo transcurrido desde la lectura anterior (milisegundos),  
% -Distancia medida por sensor Izq/Frontal (cm), 
% -Distancia medida por sensor Dch/Trasero(cm), 
% -Referencia de control (cm), 
% -Modo activo (0: Parado, 1: Control frontal, � 4),
% -Velocidad PWM motor Izq (+/-255, negativo indica marcha atr�s), 
% -Velocidad PWM motor Dch (+/-255, negativo indica marcha atr�s).

% Funci�n para mostrar telemetr�a de robot m�vil
% Laboratorio de Rob�tica 4� GIERM 18-19
% Federico Cuesta
% USO: Pasar como par�metro el nombre del archivo de telemetr�a con el
% sigiente contenido por fila:
% -Incremento de tiempo transcurrido desde la lectura anterior (milisegundos),  
% -Distancia medida por sensor Izq/Frontal (cm), 
% -Distancia medida por sensor Dch/Trasero(cm), 
% -Velocidad rad/s motor Izq,
% -Velocidad rad/s motor Dch,
% -Referencia de control (cm), 
% -Modo activo (0: Parado, 1: Control frontal, � 4),
% -Velocidad PWM motor Izq (+/-255, negativo indica marcha atr�s), 
% -Velocidad PWM motor Dch (+/-255, negativo indica marcha atr�s).

function telemetria(archivo)

tel=load(archivo);
muestras=length(tel);
disp('Incremento de tiempo m�nimo:'); disp(min(tel(:,1)));
disp('Incremento de tiempo m�ximo:');disp(max(tel(:,1)));
disp('Incremento de tiempo promedio:'); disp(mean(tel(:,1)));
tiempo=zeros(1,muestras);
tiempo(1)=tel(1,1); %Vector de tiempo absoluto
for i=2:muestras
    tiempo(i)=tiempo(i-1)+tel(i,1);
end    


subplot(2,1,1);
plot(tiempo,tel(:,2), tiempo,tel(:,3), tiempo,tel(:,4), tiempo,tel(:,5), tiempo,tel(:,6), tiempo,tel(:,7), tiempo,tel(:,8));
xlabel('Tiempo (ms)');
title('Sensores');
legend('Distancia Dch (cm)','Distancia Izq (cm)','Velocidad Izq (rad/s)','Velocidad Dch (rad/s)','Referencia (cm)','Referencia (rad/s)','�ngulo(rad)');
grid;

subplot(2,1,2);
plot(tiempo,tel(:,9), tiempo,tel(:,10), tiempo,tel(:,11));
title('Actuadores');
xlabel('Tiempo (ms)');
legend('Error','PWM Izq','PWM Dch');
grid;

end
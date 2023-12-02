#include <frc/Joystick.h>
#include <frc/TimedRobot.h>
#include <frc/motorcontrol/PWMVictorSPX.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Timer.h>
#include <frc/smartdashboard/SendableChooser.h>

class Robot : public frc::TimedRobot{

private:
    frc::Joystick g_stick{0}; // Puerto del joystick principal

    frc::PWMVictorSPX motorL_foreward{10};// Motor izquierdo frontal en el chasis
    frc::PWMVictorSPX motorL_backward{11};// Motor izquierdo trasero en el chasis
    frc::PWMVictorSPX motorR_foreward{12};// Motor derecho frontal en el chasis
    frc::PWMVictorSPX motorR_backward{13};// Motor derecho trasero en el chasis

    frc::PWMVictorSPX motorL_arm{14};// Motor izquierdo del brazo mecanico conecatdo al SPX 14
    frc::PWMVictorSPX motorR_arm{15};// Motor derecho del brazo mecanico conecatdo al SPX 15

    frc::PWMVictorSPX motorL_art{16};// Motor izquierdo de la articulacion del brazo mecanico conecatdo al SPX 16
    frc::PWMVictorSPX motorR_art{17};// Motor derecho de la articulacion del brazo mecanico conecatdo al SPX 17

    frc::PWMVictorSPX motorL_claw{18};// Motor izquierdo de la garra conecatdo al SPX 18
    frc::PWMVictorSPX motorR_claw{19};// Motor derecho de la garra conecatdo al SPX 19
    

public:
    void RobotInit() override{}

	void RobotPeriodic() override {}
 
    void AutonomousInit() override {}

    void AutonomousPeriodic() override {}

    void TeleopInit() override {}
    
	void TeleopPeriodic() override{

        // El axis 4 va a controlar el giro del robot (izquierda o derecha) mientras que el axis 2 y 3 controlaran su potencia y que avanze hacia adelante o atras
        double povX = g_stick.GetRawAxis(4);
        double pov_adelante = g_stick.GetRawAxis(3);
        double pov_atras = g_stick.GetRawAxis(2) * -1;

        // Guardar los valores de axis 1 y 5 en variables - Sus valores van del 0 a 1 y 0 a -1 pasando por decimales - Con esto se controlara el brazo
        double left_Y_Axis = g_stick.GetRawAxis(1); 
        double right_Y_Axis = g_stick.GetRawAxis(5);

        // Guardar los valores de los botones "X" y "B"
        bool botonX = g_stick.GetRawButton(2);
        bool botonB = g_stick.GetRawButton(1);

        // Movimiento del chasis
        if (pov_adelante >= 0.1){//Cuando se presione el axis3 el robot avanzara hacia adelante con la potencia dependiendo de que tan presionado este
            motorL_foreward.Set(pov_adelante);
            motorL_backward.Set(pov_adelante);
            motorR_foreward.Set(pov_adelante);
            motorR_backward.Set(pov_adelante);
        }else{// El robot se detiene
            motorL_foreward.Set(0);
            motorL_backward.Set(0);
            motorR_foreward.Set(0);
            motorR_backward.Set(0);
        }

        if (pov_atras <= 0.1){//Cuando se presione el axis2 el robot avanzara hacia atras con la potencia dependiendo de que tan presionado este
            motorL_foreward.Set(pov_atras);
            motorL_backward.Set(pov_atras);
            motorR_foreward.Set(pov_atras);
            motorR_backward.Set(pov_atras);
        }else{// El robot se detiene
            motorL_foreward.Set(0);
            motorL_backward.Set(0);
            motorR_foreward.Set(0);
            motorR_backward.Set(0);
        }

        if(povX == -1){// Cuando el povX este a la izquerda el robot gira a la izquierda en su propio eje con la misma potencia
            motorL_foreward.Set(-1);
            motorL_backward.Set(-1);
            motorR_foreward.Set(1);
            motorR_backward.Set(1);
        }else if(povX == 1){// Cuando el povX este a la izquerda el robot gira a la derecha en su propio eje con la misma potencia
            motorL_foreward.Set(1);
            motorL_backward.Set(1);
            motorR_foreward.Set(-1);
            motorR_backward.Set(-1);
        }else if(povX == 0){// Cuando el se suelta el Axis el robot igualmente se detiene
            motorL_foreward.Set(0);
            motorL_backward.Set(0);
            motorR_foreward.Set(0);
            motorR_backward.Set(0);
        }

        // Movimiento del brazo mecanico completo 
        if (left_Y_Axis == -1){ // Los siguientes if son para que el motor se acciones solo cuando el axis este a tope y no le de los valores decimales intemedios 
            motorL_arm.Set(left_Y_Axis);// Les da valor negativo para que los motores corran hacia atras por lo que el brazo se movera hacia arriba (debido a la conficguracion de engranes y cadenas)
            motorR_arm.Set(left_Y_Axis);
        }else if (left_Y_Axis == 1){
            motorL_arm.Set(left_Y_Axis);// Les da valor positivo para que los motores corran hacia adelante por lo que el brazo se movera hacia abajo (debido a la conficguracion de engranes y cadenas)
            motorR_arm.Set(left_Y_Axis);   
        }else if (left_Y_Axis == 0){// Detiene el brazo
            motorL_arm.Set(0);
            motorR_arm.Set(0);
        }

        //Movimiento de la articulacion del brazo mecanico
        double artSpeed = 0.7;// Potencia con la cual se moveran los motores de la articulacion
        if (right_Y_Axis == -1){// Cuando el axis se mueva hacia arriba la articulacion igual se mueve hacia arriba
            motorL_art.Set(artSpeed * -1);// Los dos motores de la articulacion deben de girar hacia atras para que se mueva arriba (debido a la configuracion de engranes y cadenas)
            motorR_art.Set(artSpeed * -1);
        }else if (right_Y_Axis == 1){// Los dos motores de la articulacion deben de girar hacia adelante para que se mueva abajo (debido a la configuracion de engranes y cadenas)
            motorL_art.Set(artSpeed);
            motorR_art.Set(artSpeed);
        }else if (right_Y_Axis == 0){// Detien la articulacion
            motorL_art.Set(0);
            motorR_art.Set(0);        
        }

        // Funcionamiento de la garra
        double clawSpeed = 0.5;// Potenca de los motores para abrir y cerra la garra
        if (botonX == true){// Al presionar el boton "x" los motores de las garra se moveran hacia adelante abriendo esta
            motorL_claw.Set(clawSpeed);
            motorR_claw.Set(clawSpeed);
        }else {// Al soltarlo los motores dejan de andar
            motorL_claw.Set(0);
            motorR_claw.Set(0);
        }

        if (botonB == true){// Al presionar el boton "b" los motores de las garra se moveran atras adelante cerrando esta
            motorL_claw.Set(clawSpeed * -1);
            motorR_claw.Set(clawSpeed * -1);
        }else { // Al soltarlo los motores dejan de andar
            motorL_claw.Set(0);
            motorR_claw.Set(0);
        }

    }

	void DisabledInit() override {}

    void DisabledPeriodic() override {}

    void TestInit() override {}

    void TestPeriodic() override {}

    void SimulationInit() override {}

    void SimulationPeriodic() override {}
};

#ifndef RUNNING_FRC_TESTS
int main()
{
	return frc::StartRobot<Robot>();
}
#endif


/** @file main.c
 * @author Antoine Albertelli
 * @author Florian "Flopy" Glardon
 * @date 2011
 * @brief Le fichier principal du programme.
 *
 * Ce fichier contient uniquement les fonctions de bases du code. Il se charge
 * de l'init des differents systemes et d'appeller le scheduling toutes les ms.
 */

#include <aversive.h>
#include <aversive/error.h>
#include <scheduler.h>
#include <fast_math.h>
#include <stdio.h>
#include <stdarg.h>
#include <uptime.h>
#include <string.h>
#include "commandline.h"

/* nios2.h contient toutes les fonctions dont nous avons besoin pour dialoguer
 * avec le nios alt_irq_register, IORD, etc... */
#ifdef COMPILE_ON_ROBOT
#include <nios2.h>
#endif

#include "hardware.h"
#include "cvra_cs.h"

/** Logs an event.
 *
 * This function is never called directly, but instead, the error
 * modules fills an error structure and calls it.
 * @param [in] e The error structure, filled with every needed info.
 */
void mylog(struct error * e, ...) {
	va_list ap;
	va_start(ap, e);	
    /* Prints the filename (not the full path) and line number. */
    printf("%s:%d ", strrchr(e->file, '/') ? strrchr(e->file, '/')+1:e->file, e->line);
	vprintf(e->text, ap);
	printf("\r\n");
	va_end(ap);
}
/** Cette variable contient le temps maximum passe dans une boucle du scheduler.
 * Elle donne donc une assez bonne indication de l'occupation du CPU. */
int32_t longest_scheduler_interrupt_time=0;

#ifdef COMPILE_ON_ROBOT
/** Cette fonction est appellee a chaque overflow du timer (toutes les ms).
 * @param param Unused parameter, but requested by the NIOSII API.
 */
void main_timer_interrupt(__attribute__((unused)) void *param) {
	static int i=0;
	int32_t time;

	/* Chenillard sur les LEDs */
	i++;
	IOWR(LED_BASE, 0, i/100);
	if(i==0xfe*100)
	   	i=0;

	/* Reset le timer. */
	IOWR(TICK_BASE, 0, 0x00); 

	/* Execute les taches programmees en mesurant le temps mis. */
	time = uptime_get(); 
	scheduler_interrupt();
	time = uptime_get() - time;

	/* Si le temps est plus grand que le maximum, on le garde en memoire. */
	if(time > longest_scheduler_interrupt_time) {
	 longest_scheduler_interrupt_time = time;
	}
}
#endif

/** @brief Demarre le robot
 *
 * Cette fonction est la premiere executee par notre code. Elle est responsable
 * de l'initialisation de tous les sous-systemes dans un ordre permettant leur
 * bon fonctionnement.
 *
 * @bug Est-ce qu'on peut se debarasser de argc et argv ? antoine
 * @param argc, argv Unused parameters, but requested by the Nios II API.
 * @returns Never returns 
 */ 
int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {

	robot.verbosity_level = ERROR_SEVERITY_NOTICE;
    
	/* Step 1 : Setup UART speed. Doit etre en premier car necessaire pour le log. */
	//cvra_set_uart_speed(COMPC_BASE, 57600);
    //
    error_register_emerg(mylog);
    error_register_error(mylog);
    error_register_warning(mylog);
    error_register_notice(mylog);
    error_register_debug(mylog);

#ifdef COMPILE_ON_ROBOT
	cvra_set_uart_speed(COMPC_BASE, 57600); 
#endif
    //commandline_init();
    //for(;;) commandline_input_char(getchar());

	/* Step 2 : Init de la librairie math de Mathieu. */
    /**FIXME @todo Est-ce qu'on a encore besoin de fast_math_init() dans la version finale ? */
    fast_math_init();


	/* Step 3 : Demare le scheduler pour le multitache. */
	scheduler_init(); 

#ifdef COMPILE_ON_ROBOT
	/* Step 3 (suite) : Si on est sur le robot on inscrit le tick dans la table des interrupts. */
	alt_ic_isr_register(0, TICK_IRQ, main_timer_interrupt, NULL, 0);
	sei(); /** FIXME @todo sei() Necessaire ? */
#endif

	/* Step 4 : Init les IO. */
	cvra_board_init();
	
	/* Step 5 : Init la regulation et l'odometrie. */
	//cvra_cs_init();  // Desactive depuis la casse du moteur.
	
	/* Init les bras.  */
    arm_init(&robot.left_arm);
    robot.left_arm.offset_xy.x = 20.;
    robot.left_arm.offset_xy.y = 10.;
    robot.left_arm.offset_rotation = 1.5707;

	/* Step 7 : Init tout les parametres propres a une certaine edition ainsi que l'evitement d'obstacle. */
    /** FIXME @todo Init des parametres robot a refaire au propre. */


	/* Step 8 : Demarre la comm avec le PC, pas de retour de cette fonction. */
    commandline_init();
    for(;;) commandline_input_char(getchar());
    	
	return 0;
}


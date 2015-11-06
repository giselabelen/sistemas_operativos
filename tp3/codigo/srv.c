#include "srv.h"

/*
 *  Ejemplo de servidor que tiene el "sí fácil" para con su
 *  cliente y no se lleva bien con los demás servidores.
 *
 */

void servidor(int mi_cliente)
{
    MPI_Status status; int origen, tag;
    int hay_pedido_local = FALSE;
    int listo_para_salir = FALSE;
    // CANTIDAD DE GENTE QUE ME PUEDE HABLAR (mpi_size)
    int cant_srv;
    MPI_Comm_size(MPI_COMM_WORLD, &cant_srv);
    cant_srv = cant_srv/2;
    // MI SEQUENCE NUMBER(inicializame en 0?)
    int mi_seq_num = 0;
    // EL SEQUENCE NUMBER MAS ALTO(inicializame en 0?)
    int highest_seq_num = 0;
    // CONTESTACIONES FALTANTES(inicializame en 0?)
    int reply_faltantes = 0;
    // SECCION CRITICA OCUPADA(inicializame en FALSE?)
    //~ int sec_critica_ocupada = FALSE;
    // ARREGLO DE DIFERIR RTA
    int i;
    int diferidos[cant_srv];
    for(i = 0; i < cant_srv; i++) { diferidos[i] = FALSE; }
    int msj;
    
    while( ! listo_para_salir ) {
        
        MPI_Recv(&msj, 1, MPI_INT, ANY_SOURCE, ANY_TAG, COMM_WORLD, &status);
        origen = status.MPI_SOURCE;
        tag = status.MPI_TAG;
        
        if (tag == TAG_PEDIDO) {
            assert(origen == mi_cliente);
            debug("Mi cliente solicita acceso exclusivo");
            assert(hay_pedido_local == FALSE);
            hay_pedido_local = TRUE;
            debug("Dándole permiso (frutesco por ahora)");
            
            // ACA HACER LA MAGIA
            /* SEQUENCE NUMBER MAS ALTO + 1 
             * PONE EN N-1 CONTESTACIONES FALTANTES
             * (SE FIJA MANDARLE QUIZAS A LOS SERVERS PRENDIDOS)
             * MANDO EL PEDIDO A TODOS CON ESE SEQUENCE NUMBER Y TAG_QUIERO_ACCESO Y NUMERO DE SERVER
             * EL MPI SEND NO SE HACE ACA, SE DEBE ESPERAR CONFIRMACION
             * */
            mi_seq_num = highest_seq_num + 1;
            reply_faltantes = cant_srv - 1;
            for(i = 0; i < cant_srv; i++)
            {
				if(2*i != mi_cliente - 1)
				{
					MPI_Send(&mi_seq_num, 1, MPI_INT, 2*i, TAG_QUIERO_ACCESO, COMM_WORLD);
				}
			}
            
            //~ MPI_Send(NULL, 0, MPI_INT, mi_cliente, TAG_OTORGADO, COMM_WORLD);
        }
        
        else if (tag == TAG_LIBERO) {
			/*DESOCUPAR SECCION CRITICA
			 * MANDAR OK A TODOS LOS DIFERIDOS
			 * */
            assert(origen == mi_cliente);
            debug("Mi cliente libera su acceso exclusivo");
            assert(hay_pedido_local == TRUE);
            hay_pedido_local = FALSE;
            
            //~ sec_critica_ocupada = FALSE;
            for(i = 0; i < cant_srv; i++)
            {
				if(diferidos[i])
				{
					MPI_Send(NULL, 0, MPI_INT, 2*i, TAG_POR_MI_ACCEDE, COMM_WORLD);
				}
			}
            // AVISAR AL RESTO DE LOS SRV
        }
        
        else if (tag == TAG_TERMINE) {
			/*(AVISAR QUE ME APAGO A LOS SERVER)
			 * */
            assert(origen == mi_cliente);
            debug("Mi cliente avisa que terminó");
            //~ listo_para_salir = TRUE;
        }
        
        // AGREGAR LOS CASOS EN QUE RECIVE MSJS DE SERVIDORES
        
        /*RUTINA PARA TAG_QUIERO_ACCESO*/
        else if (tag == TAG_QUIERO_ACCESO) {
         /* ACTUALIZAR EL NUMERO MAS ALTO
          * ACTUALIZAR EL VALOR DE DIFERIDO O NO PARA ESE SERVER SEGUN LAS REGLAS DE LA TABLITA
          * SI NO LO DIFIERO CONTESTO AL SERVIDOR CON TAG_DALE_QUE_VA
          * */
			if (msj > highest_seq_num){	highest_seq_num = msj; }
			
			if (hay_pedido_local && (msj > mi_seq_num || (msj == mi_seq_num && mi_cliente-1 < origen)))
			{
				diferidos[origen/2] = TRUE;
			}
			else{ MPI_Send(NULL, 0, MPI_INT, origen, TAG_POR_MI_ACCEDE, COMM_WORLD); }
        }
          //RUTINA PARA EL TAG DALE_QUE_VA
        else if (tag == TAG_POR_MI_ACCEDE) {
          /*DECREMENTAR LA CUENTA DE CONTESTACIONES FALTANTES
           * OCUPAR LA SECCION CRITICA
           * SI ES 0 AVISARLE AL CLIENTE CON EL TAG_OTORGADO
           * */
			reply_faltantes--;
			
			if (reply_faltantes == 0){
				//~ sec_critica_ocupada = TRUE;
				MPI_Send(NULL, 0, MPI_INT, mi_cliente, TAG_OTORGADO, COMM_WORLD);
			}
        }
    }
    
}


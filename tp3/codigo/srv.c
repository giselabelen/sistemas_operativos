#include "srv.h"

void servidor(int mi_cliente)
{
    MPI_Status status; int origen, tag;
    int hay_pedido_local = FALSE;
    int listo_para_salir = FALSE;
    int i;
    int msj;

    int cant_srv;	// cantidad de servidores originales (antes de que empiecen a apagarse)
//    MPI_Comm_size(MPI_COMM_WORLD, &cant_srv);
//    cant_srv = cant_srv/2;
	cant_srv = cant_ranks/2;
    
    int mi_seq_num = 0;	// mi sequence number
    
    int highest_seq_num = 0;	// sequence number más alto hasta ahora
    
    int reply_faltantes = 0;	// contestaciones faltantes (para cuando pido acceso exclusivo)
    
    int diferidos[cant_srv];	// arreglo que dice TRUE en la posición del servidor al que le difiero el reply
    for(i = 0; i < cant_srv; i++) { diferidos[i] = FALSE; }
        
    int resp_apagado = 0;	// respuestas faltantes para poder apagarme
    
    int apagados[cant_srv];	// arreglo que dice TRUE en la posición del servidor que ya se apagó
    for(i = 0; i < cant_srv; i++) { apagados[i] = FALSE; }
    
    int cant_srv_activos = cant_srv;	// cantidad de servidores prendidos en este momento
    
    while( ! listo_para_salir ) {
        
        MPI_Recv(&msj, 1, MPI_INT, ANY_SOURCE, ANY_TAG, COMM_WORLD, &status);
        origen = status.MPI_SOURCE;
        tag = status.MPI_TAG;
        
        if (tag == TAG_PEDIDO) {
			// Mi cliente me pide acceso exclusivo
            assert(origen == mi_cliente);
            debug("Mi cliente solicita acceso exclusivo");
            assert(hay_pedido_local == FALSE);
            hay_pedido_local = TRUE;
            debug("Dándole permiso");
            
            mi_seq_num = highest_seq_num + 1;
            reply_faltantes = cant_srv_activos - 1;	// voy a esperar a que los srvs prendidos (menos yo) me den el ok
            if (reply_faltantes == 0){
                // si no hay mas srvs que yo, ya le doy el ok a mi cliente
                MPI_Send(NULL, 0, MPI_INT, mi_cliente, TAG_OTORGADO, COMM_WORLD);
            }else{
				// si no, aviso a todos los srvs (menos yo)
                for(i = 0; i < cant_srv; i++)
                {
    				if((2*i != mi_cliente - 1) && !apagados[i])
    				{
    					MPI_Send(&mi_seq_num, 1, MPI_INT, 2*i, TAG_QUIERO_ACCESO, COMM_WORLD);
    				}
    			}
            }
        }
        
        else if (tag == TAG_LIBERO) {
			// Mi cliente libera su acceso exclusivo
			assert(origen == mi_cliente);
            debug("Mi cliente libera su acceso exclusivo");
            assert(hay_pedido_local == TRUE);
            hay_pedido_local = FALSE;
            
            // doy el ok a los srvs que habia diferido
            for(i = 0; i < cant_srv; i++)
            {
				if(diferidos[i])
				{
                    assert(!apagados[i]);
                    diferidos[i] = FALSE;
					MPI_Send(NULL, 0, MPI_INT, 2*i, TAG_POR_MI_ACCEDE, COMM_WORLD);
				}
			}
        }
        
        else if (tag == TAG_TERMINE) {
			// Mi cliente terminó
            assert(origen == mi_cliente);
            debug("Mi cliente avisa que terminó");
            assert(!hay_pedido_local);
            
            resp_apagado = cant_srv_activos - 1;	// voy a tener que esperar que todos los srvs prendidos (menos yo) me confirmen
            if (resp_apagado == 0)
            {	// si soy el unico srv prendido me apago nomas
				listo_para_salir = TRUE; 
			} 
            else{
				// si no, aviso a todos los srvs prendidos (menos yo) que me apago
                for(i = 0; i < cant_srv; i++)
                {
                    if(!apagados[i] && (2*i != mi_cliente-1))
                    {
                        MPI_Send(NULL, 0, MPI_INT, 2*i, TAG_ME_APAGO, COMM_WORLD);
                    }
                }
            }
        }
        
        else if (tag == TAG_QUIERO_ACCESO) {
			// un srv me pide acceso exclusivo
			assert(origen%2==0);
			debug("alguien pidio acceso");
			//~ assert(msj>=mi_seq_num);
			
			if (msj > highest_seq_num){	highest_seq_num = msj; }	// actualizo sequence number más alto
			
			// veo si lo difiero o si le doy el ok (esto sale del paper)
			if (hay_pedido_local && (msj > mi_seq_num || (msj == mi_seq_num && mi_cliente-1 < origen)))
			{
				diferidos[origen/2] = TRUE;
			}else{ 
				MPI_Send(NULL, 0, MPI_INT, origen, TAG_POR_MI_ACCEDE, COMM_WORLD); 
			}
        }
        
        else if (tag == TAG_POR_MI_ACCEDE) {
			// un srv me da el ok para mi acceso exclusivo
			debug("alguien me dio permiso");
			assert(hay_pedido_local);
			
			reply_faltantes--;	// actualizo la cantidad de respuestas faltantes
			
			if (reply_faltantes == 0){
				// si ya me contestaron todos, le aviso a mi cliente
				MPI_Send(NULL, 0, MPI_INT, mi_cliente, TAG_OTORGADO, COMM_WORLD);
			}
        }

        else if (tag == TAG_ME_APAGO) {
			// un srv me dice que se va a apagar
			debug("alguien se va");
			
			// me anoto que se apaga
            apagados[origen/2] = TRUE;
            cant_srv_activos--;
            
            // le doy mi ok para que se apague
            MPI_Send(NULL, 0, MPI_INT, origen, TAG_OK_APAGATE, COMM_WORLD);
        }

        else if(tag == TAG_OK_APAGATE){
			// un srv me da el ok para apagarme
			debug("me llega un ok para apagarme");
			
            resp_apagado--;	// ahora espero uno menos
            
            if (resp_apagado == 0){ listo_para_salir = TRUE; }	// si ya me confirmaron todos, me apago
        }
    }   
}

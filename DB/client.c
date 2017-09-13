/*
** File:  client4.c
*/

#include <stdlib.h>
#include <string.h>
#include "libpq-fe.h"
#include "readline/readline.h"
#include "readline/history.h"
typedef enum { FALSE, TRUE } bool;
#define MAX_PRINT_LEN 40
static char separator[MAX_PRINT_LEN+1];

void print_result_set( PGresult * result ){
	int col;
	int row;
 	int	*sizes;

/*
** Compute the size for each column
*/

	sizes = (int *)calloc( PQnfields( result ), sizeof( int ));
	for( col = 0; col < PQnfields( result ); col++ ){
 		int len = 0;
		for( row = 0; row < PQntuples( result ); row++ ){
 			if( PQgetisnull( result, row, col ))
 	         	len = 0;
        	else
            	len = PQgetlength( result, row, col );
        	if( len > sizes[col] )
          		sizes[col] = len;
     	}
 
		if(( len = strlen( PQfname( result, col ))) > sizes[col] )
 	       sizes[col] = len;
	    if( sizes[col] > MAX_PRINT_LEN )
 	       sizes[col] = MAX_PRINT_LEN;
   }

 /*
 ** Print the field names.
 */

	for( col = 0; col < PQnfields( result ); col++ ){
 		printf( "%-*s ", sizes[col], PQfname( result, col ));
 	}
 		printf( "\n" );
/*
** Print the separator line
*/

	memset( separator, '-', MAX_PRINT_LEN );
	for( col = 0; col < PQnfields( result ); col++ ){
 		printf( "%*.*s ", sizes[col], sizes[col], separator );
	}
		printf( "\n" );
/*
** Now loop through each of the tuples returned by
** our query and print the results.
*/
	for( row = 0; row < PQntuples( result ); row++ ){
 		for( col = 0; col < PQnfields( result ); col++ ){
			if( PQgetisnull( result, row, col ))
				printf( "%*s |", sizes[col], "" );
 	       else
		   		printf( "%*s |", sizes[col], PQgetvalue( result, row, col ));
 		}
			printf( "\n" );
	}
 	printf( "(%d rows)\n\n", PQntuples( result ));
	free( sizes );
 }

/*void process_query(PGconn * connection,
  const char * query_text) {
  PGresult * result;
  if ((result = PQexec(connection, query_text)) == NULL) {
    printf("%s\n", PQerrorMessage(connection));
    return;
  }
  if (PQresultStatus(result) == PGRES_TUPLES_OK) {
    print_result_set(result);
  } else if (PQresultStatus(result) == PGRES_COMMAND_OK) {
    printf("%s", PQcmdStatus(result));
    if (strlen(PQcmdTuples(result)))
      printf(" - %s rows\n\n", PQcmdTuples(result));
    else
      printf("\n\n");
  } else {
    printf("%s\n\n", PQresultErrorMessage(result));
  }
  PQclear(result);
}*/

#if 0
void print_native(PGresult *result){
	PQprintOpt        options = {0};

	options.header    = 1;    /* Ask for column headers            */
	options.align     = 1;    /* Pad short columns for alignment   */
	options.fieldSep  = "|";  /* Use a pipe as the field separator */	
	
	PQprint(stdout, result, &options);
	PQclear(result);
}
#endif

void insert_from(PGconn * conn){
	PGresult *result;
	printf("NOC1 TABLE: Table with ID Label\n");
	result = PQexec(conn, "DROP TABLE NOC;");
	result = PQexec(conn, "CREATE TABLE NOC(id int, SRC_INSTANCE varchar(80), SRC_PORT varchar(80), SYNC_INSTANCE varchar(80), SYNC_PORT varchar(80));");
	result = PQexec(conn, "COPY NOC FROM '/proj/xsjhdstaff4/rsirasao/runsdir/gemm_demo/new/xnwOut/channel.id' DELIMITERS ',' CSV;");	
	result = PQexec(conn, "SELECT * FROM NOC");
	print_result_set(result);
	PQclear(result);

	printf("NOC2 TABLE: Table with Data\n");
	result = PQexec(conn, "DROP TABLE NOC2;");
	result = PQexec(conn, "CREATE TABLE NOC2(id int, time varchar(80), pkt_id int, pkt_type int, header int, vc int, src int, dest int);");
	result = PQexec(conn, "COPY NOC2 FROM '/proj/xsjhdstaff4/rsirasao/runsdir/gemm_demo/new/xnwOut/noc.channels' DELIMITERS ',' CSV;");	
	//result = PQexec(conn, "COPY NOC2 FROM '/proj/xsjhdstaff1/deveshn/nobkup/issues/NOC_graph/runs/simulation/psql/xnwOut/noc.channels' DELIMITERS ',' CSV;");	
	result = PQexec(conn, "SELECT * FROM NOC2");
	print_result_set(result);
	PQclear(result);
}


void process_query( PGconn * connection, const char * query_text ){
	PGresult *result;
	if( PQsendQuery( connection, query_text ) == 0 ){ //send command string to server using this
	   printf( "%s\n", PQerrorMessage( connection )); //return immediately after queuing command
		return;
	}
	
	while(( result = PQgetResult( connection )) != NULL ){ //this obtains result from server 
		if( PQresultStatus( result ) == PGRES_TUPLES_OK ){
			print_result_set( result );
	}else if( PQresultStatus( result ) == PGRES_COMMAND_OK ){
		printf( "%s", PQcmdStatus( result ));
		if( strlen( PQcmdTuples( result )))
			printf( " - %s rows\n", PQcmdTuples( result ));
		else
		printf( "\n" );
	}
	else{
		printf( "%s\n", PQresultErrorMessage( result ));
	}
		PQclear( result );
	}
}


int main(int argc, char * argv[]) {
  PGconn * conn;
  char * buf;
  const char *conninfo;

  //connection = PQconnectdb(argc > 1 ? argv[1] : "");
   if (argc > 1)
        conninfo = argv[1];
       	//conninfo = (argc > 1 ? argv[1] : ""); 
    else
        conninfo = "dbname = nocdb2";
	/* Make a connection to the database */
    conn = PQconnectdb(conninfo);

    /* Check to see that the backend connection was successfully made */

    if (PQstatus(conn) != CONNECTION_OK){
        fprintf(stderr, "Connection to database failed: %s",
                PQerrorMessage(conn));
    	PQfinish(conn);
		exit(1);	
    }

     if (conn == NULL) {
    	printf("Fatal error - unable to allocate connection\n");
 		exit(EXIT_FAILURE);
  	}

  /*if (PQstatus(conn) == CONNECTION_OK) {
    using_history();
    read_history(".pg_history");

    while ((buf = readline("-->")) != NULL) {
      if (strncmp(buf, "quit", sizeof("quit") - 1) == 0) {
        break;
      } else {
        if (strlen(buf) != 0) {
          add_history(buf);
          process_query(conn, buf);
        }
        free(buf);
      }
    }
    //err = write_history(".pg_history");
  } else {
    printf("%s\n", PQerrorMessage(conn));

  }*/

  insert_from(conn);
  PQfinish(conn);
  exit(EXIT_SUCCESS);
}

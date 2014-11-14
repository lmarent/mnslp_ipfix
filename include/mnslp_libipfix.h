

int mnslp_ipfix_export( ipfix_t *ifh, 
					    mnslp_ipfix_message_t *mes, 
					    ipfix_template_t *templ, ... );
					    
int _mnslp_ipfix_export_array( ipfix_t          *ifh,
							  ipfix_template_t *templ,
							  mnslp_ipfix_message_t *mes, 
							  int              nfields,
							  void             **fields,
							  uint16_t         *lengths );

void allocate_additional_memory(mnslp_ipfix_message_t *mes, 
								size_t additional);

int _mnslp_ipfix_write_template( ipfix_t           *ifh,
								 mnslp_ipfix_message_t *mes,
								 ipfix_template_t  *templ );

/* Name      : do_writen
 * Parameter : > mes   		output message
 *             > *ptr       buffer
 *             > nbytes     number of bytes to write
 * Purpose   : write 'n' bytes to message
 * Returns   : number of written bytes.
 */
static int _mnslp_do_writen( mnslp_ipfix_message_t *mes, char *ptr, int nbytes );

int _mnslp_ipfix_send_msg( ipfix_t *ifh, 
						   mnslp_ipfix_message_t *mes, 
						   iobuf_t *buf );

int _mnslp_ipfix_write_hdr( ipfix_t *ifh, 
							iobuf_t *buf );

int _mnslp_ipfix_export_flush( ipfix_t *ifh, 
							   mnslp_ipfix_message_t *mes );

int _mnlsp_ipfix_export_array( ipfix_t          *ifh,
							   ipfix_template_t *templ,
							   mnslp_ipfix_message_t *mes,
							   int              nfields,
							   void             **fields,
							   uint16_t         *lengths );



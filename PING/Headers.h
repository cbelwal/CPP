	// IP HEADER

struct IP_HEADER
{
	u_char ip_hl;			// Header Length         -> Bytes = 1
	u_char ip_v;			// Version               -> Bytes = 1
	u_char ip_tos;			// Type of service       -> Bytes = 1
	short ip_len;			// Total length			 -> Bytes = 2
	u_short ip_id;			// Identification		 -> Bytes = 1
	short ip_off;			// Fragment Offset       -> Bytes = 2

	u_char ip_ttl;			// Time to live          -> Bytes = 1
	u_char ip_p;			// Protocol				 -> Bytes = 1

	u_short ip_cksum;		// Checksum				 -> Bytes = 2

	struct in_addr ip_src;  // Source Address		 -> Bytes = 4
	struct in_addr ip_dst;  // Destination Address   -> Bytes = 4
};

	// ICMP HEADER ( PIGGYBACK ON IP )

struct ICMP_HEADER
{
	u_char icmp_type;
	u_char icmp_code;
	u_short icmp_cksum;
	u_short icmp_id;
	u_short icmp_seq;
	char icmp_data[1]; //1 bytes long. Will store time
};


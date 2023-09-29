#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <arpa/inet.h>
#include <math.h>

#define PREFIX_LEN 15
#define MAX_MASK_LEN 32

const unsigned int FMASK = 0xFFFFFFFF;
#define UNSET_BIT(n, pos) (n = n & ((1 << pos) ^ FMASK))
#define COMPLIMENT(n) (n = n ^ FMASK)

static unsigned int get_mask_value_in_integer_format(char mask_value)
{
  unsigned int mask = FMASK;
  int n_tail_bits = MAX_MASK_LEN - mask_value;

  for (int i = 0; i < n_tail_bits; i++)
  {
    UNSET_BIT(mask, i);
  }
  return mask;
}

unsigned int get_ip_integer_equivalent(char *ip_address)
{
  unsigned int ip_int = 0;
  inet_pton(AF_INET, ip_address, &ip_int);
  return htonl(ip_int);
}

void get_broadcast_address(char *ip_addr, char mask, char *output_buffer)
{
  unsigned int ip_int = get_ip_integer_equivalent(ip_addr);
  unsigned int mask_int = get_mask_value_in_integer_format(mask);
  COMPLIMENT(mask_int);

  unsigned int broadcast_addr = ip_int | mask_int;
  broadcast_addr = htonl(broadcast_addr);

  inet_ntop(AF_INET, &broadcast_addr, output_buffer, PREFIX_LEN + 1);
  output_buffer[PREFIX_LEN] = '\0';
}

void get_abcd_ip_format(unsigned int ip_address, char *output_buffer)
{
  inet_ntop(AF_INET, &ip_address, output_buffer, PREFIX_LEN + 1);
  output_buffer[PREFIX_LEN] = '\0';
}

void get_network_id(char *ip_address, char mask, char *output_buffer)
{
  unsigned int ip_int = get_ip_integer_equivalent(ip_address);

  unsigned int mask_int = get_mask_value_in_integer_format(mask);

  unsigned int network_id = ip_int & mask_int;
  network_id = htonl(network_id);

  inet_ntop(AF_INET, &network_id, output_buffer, PREFIX_LEN + 1);
  output_buffer[PREFIX_LEN] = '\0';
}

unsigned int get_subnet_cardinality(char mask_value)
{
  return pow(2, (MAX_MASK_LEN - mask_value)) - 2;
}

int check_ip_subnet_membser_ship(char *network_id, char mask, char *check_ip)
{
  unsigned int ip_int = get_ip_integer_equivalent(check_ip);
  unsigned int mask_int = get_mask_value_in_integer_format(mask);

  unsigned int calc_nw_id = ip_int & mask_int;
  calc_nw_id = htonl(calc_nw_id);

  unsigned int nw_ip = get_ip_integer_equivalent(network_id);
  nw_ip = htonl(nw_ip);

  if (nw_ip == calc_nw_id)
  {
    return 0;
  }

  return -1;
}

int main(int argc, char **argv)
{

  /*Testing get_broadcast_address()*/
  {
    printf("Testing Q1..\n");
    char ip_address[PREFIX_LEN + 1],
        output_buffer[PREFIX_LEN + 1];
    memset(ip_address, 0, PREFIX_LEN + 1);
    memcpy(ip_address, "192.168.2.10", strlen("192.168.2.10"));
    ip_address[strlen(ip_address)] = '\0';
    char mask = 24;
    memset(output_buffer, 0, PREFIX_LEN + 1);
    get_broadcast_address(ip_address, mask, output_buffer);
    printf("broadcast address  = %s\n", output_buffer);
    printf("Testing Q1 Done.\n");
  }

  /*Testing get_ip_integer_equivalent()*/
  {
    printf("Testing Q2..\n");
    char ip_address[PREFIX_LEN + 1];
    memset(ip_address, 0, PREFIX_LEN + 1);
    memcpy(ip_address, "192.168.2.10", strlen("192.168.2.10"));
    ip_address[strlen(ip_address)] = '\0';
    unsigned int a = get_ip_integer_equivalent(ip_address);
    printf("a = %u\n", a);
    printf("Testing Q2 Done.\n");
  }

  /*Testing get_abcd_ip_format()*/
  {
    char output_buffer[PREFIX_LEN + 1];
    memset(output_buffer, 0, PREFIX_LEN + 1);
    unsigned int a = 2058138165;
    printf("Testing Q3..\n");
    get_abcd_ip_format(htonl(a), output_buffer);
    printf("IP address in A.B.C.D format = %s\n", output_buffer);
    printf("Testing Q3 Done.\n");
  }

  /*Testing get_network_id()*/
  {
    printf("Testing Q4..\n");
    char ip_address[PREFIX_LEN + 1],
        output_buffer[PREFIX_LEN + 1];
    memset(ip_address, 0, PREFIX_LEN + 1);
    memcpy(ip_address, "192.168.2.10", strlen("192.168.2.10"));
    ip_address[strlen(ip_address)] = '\0';
    char mask = 20;
    memset(output_buffer, 0, PREFIX_LEN + 1);
    get_network_id(ip_address, mask, output_buffer);
    printf("Network Id = %s/%u\n", output_buffer, mask);
    printf("Testing Q4 Done.\n");
  }

  /*Testing get_subnet_cardinality() */
  {
    printf("Testing Q5..\n");
    char mask = 24;
    printf("Cardinality = %u\n", get_subnet_cardinality(mask));
    printf("Testing Q5 Done.\n");
  }

  {
    /*Testing check_ip_subnet_membser_ship()*/
    printf("Testing Q6..\n");
    char network_id[PREFIX_LEN + 1];
    strncpy(network_id, "192.168.0.0", strlen("192.168.0.0"));
    network_id[PREFIX_LEN] = '\0';

    char mask = 16;

    char ip_address[PREFIX_LEN + 1];
    strncpy(ip_address, "192.168.1.10", strlen("192.168.1.10"));

    int res = check_ip_subnet_membser_ship(network_id, mask, ip_address);
    printf("IP Subnet check Result = %s\n", res == 0 ? "Membership true" : "Membership false");
    printf("Testing Q6 Done.\n");
  }

  return 0;
}

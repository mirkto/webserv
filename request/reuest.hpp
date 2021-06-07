#include "iostream"
#include "map"

struct Reuest {
	std::string buf;
	std::string	_type_reuest;
	std::string	_resource_name;
	std::string	_protocol_version;
	std::string _body;
	int			_f_sl_status;
	int			_f_hd_status;
	int			_f_bd_status;
	int			_f_end_reuest;
	std::map<std::string, std::string> headers;
	int index;
	int i_end;
	int len;
	int f_error;
	int method_body;
	int find_end(std::string end, int i);
	void new_buf();
	void filling_start_line();
	void filling_headers();
	void filling_body_length();
	void filling_body_encoding();
	void reuest_init();
	void func_find_end();
	void func_reuest(std::string str);
	void see_reuest();
	int  hex_to_dec(std::string st);
	int str_to_un_int(std::string str);
};
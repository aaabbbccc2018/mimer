#include <string>
#include "mysqler.h"

bool mysqler::_embedded;

mysqler::mysqler(const char *host, const char *user,
				 const char *password, const char *db,
				 uint port, const char *unixSocket, ulong clientFlags) :
	             _con(NULL), _result(NULL), _host(host), _user(user),
				 _password(password), _database(db), _usocket(unixSocket),
				 _port(port), _cliflag(clientFlags),_isConnected(false),
				 _autocommit(true)
{
	_log = new mim::ellog("mysqler", "./logs");
	if ((_con = mysql_init(NULL)) == NULL) {
		finish_with_error("Error tryng to initialize MYSQL mysqler insufficient memory");
	}
	if (_embedded)
	{
		mysql_options(_con, MYSQL_READ_DEFAULT_GROUP, "embedded");
		mysql_options(_con, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);
	}
	_log->info("host: %v user: %v database: %v  embedded status: %v" ,_host ,_user ,_database ,_embedded);
}

mysqler::mysqler() :_con(NULL), _result(NULL), _host(NULL), _user(NULL), _password(NULL),
                    _database(NULL), _usocket(NULL), _port(0), _cliflag(0),
                    _isConnected(false),_autocommit(true)
{
	if ((_con = mysql_init(NULL)) == NULL)
	{
		finish_with_error("Error tryng to initialize MYSQL mysqler insufficient memory");
	}
	if (_embedded)
	{
		mysql_options(_con, MYSQL_READ_DEFAULT_GROUP, "embedded");
		mysql_options(_con, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);
	}
}

mysqler::~mysqler()
{
	if (_con != NULL)
	{
		mysql_close(_con);
		_con = NULL;
	}
	if (_result != NULL)
	{
		mysql_free_result(_result);
		_result = NULL;
	}
}

bool mysqler::connected() const
{
	return _isConnected;
}

bool mysqler::connect(const char* charset)
{
	if (!_host)
	{
		_log->info("connect host: %v charset: %v", _host, charset);
		if (mysql_real_connect(_con, NULL, NULL, NULL, _database, 0, NULL, 0) == NULL)
		{
			finish_with_error("Error in mysqler::Connect");
			return false;
		}
	}
	else
	{
		if (mysql_real_connect(_con, _host, _user, _password, _database, _port, _usocket, _cliflag) == NULL)
		{
			finish_with_error("Error in mysqler::Connect");
			return false;
		}
	}
	_isConnected = true;
	if (mysql_set_character_set(_con, charset) != 0)
	{
		finish_with_error("Error in DatabaseConnect");
		return false;
	}
	return true;
}

void mysqler::login(const char * user, const char * password)
{
	if (_con && user && password)
	{
		_user = user;
		_password = password;
	}
}

bool mysqler::usedb(const char *db)
{
	_log->info("use database: %v", db);
	if (mysql_select_db(_con, db) != 0)
	{
		finish_with_error("Error in mysqler::usedb");
		return false;
	}
	return true;
}

int mysqler::execsql(const char* statement)
{
	_log->info("execute statement: %v", statement);
	if (mysql_query(_con, statement) != 0)
	{
		finish_with_error("Error in mysqler::Execute - query");
	}
	_result = mysql_store_result(_con);
	return (int)mysql_affected_rows(_con);
}

void mysqler::show_result()
{
	if (_result != NULL)
	{
		int num_fields = mysql_num_fields(_result);
		MYSQL_ROW row;
		while ((row = mysql_fetch_row(_result)))
		{
			for(int i = 0; i < num_fields; i++)
			{
				printf("%s ", row[i] ? row[i] : "NULL");
				_log->debug("%s: %v", row[i] ? row[i] : "NULL");
			}
			printf("\n");
		}
	}
}

void mysqler::free_result()
{
	if (_result != NULL)
	{
		mysql_free_result(_result);
		_result = NULL;
	}
}

void mysqler::autocommit(bool chose)
{
	int ac = (chose) ? 1 : 0;
	if (mysql_autocommit(_con, ac) != 0)
	{
		finish_with_error("Error in mysqler::Autocommit");
	}
}

void mysqler::startTransaction()
{
	if (mysql_query(_con, "START TRANSACTION") != 0)
	{
		finish_with_error("Error in mysqler::StartTransaction");
	}
}

void mysqler::commit()
{
	if (mysql_query(_con, "COMMIT") != 0)
	{
		finish_with_error("Error in mysqler::Commit");
	}
}

void mysqler::rollback()
{
	if (mysql_query(_con, "ROLLBACK") != 0)
	{
		finish_with_error("Error in mysqler::Rollback");
	}
}

void mysqler::initLibrary(bool embedded)
{
	_embedded = embedded;
	if (! embedded)
	{
		if (mysql_library_init(0, NULL, NULL) != 0) {
			finish_with_error("Error in Database::Initialize Failure to initialize the database library");
		}
	}
	else
	{
		static const char *server_args[] = {
			"this_program", "--basedir=/usr/local/mysql", "--datadir=/Users/ravidesai/mysql/data", 
			"--plugin-dir=/Users/ravidesai/mysql/plugins", "--log-error=/Users/ravidesai/mysql/tmp/test.err",
			"--pid-file=/Users/ravidesai/mysql/tmp/test.pid",
			"--key_buffer_size=32M", "--log-bin=/Users/ravidesai/mysql/log/logbin"
				"--log-bin-trust-function-creators=1"
				"--log-bin-trust-routine-creators=1"
		};
		static const char *server_groups[] = { "embedded", "server", "this_program_SERVER", (char *) NULL };
		if (mysql_library_init(sizeof(server_args) / sizeof(char *), (char**) server_args, (char **)server_groups) != 0) {
			finish_with_error("Error in Database::Initialize Failure to initialize the database library");
		}
	}
}

void mysqler::initThread()
{
	if (mysql_thread_init() != 0) {
		finish_with_error("Error in Database::ThreadInitialize Failure to initialize database for this thread");
	}
}

void mysqler::finaLibrary()
{
	mysql_library_end();
}

void mysqler::finaThread()
{
	mysql_thread_end();
}

void mysqler::finish_with_error(const char *desc)
{
	if (NULL == desc)
		desc = "";
	fprintf(stderr, "%s [%d]%s\n", desc, mysql_errno(_con), mysql_error(_con));
	_log->error("%v [%v]%v", desc, mysql_errno(_con), mysql_error(_con));
	if (_con)
	{
		mysql_close(_con);
		_con = NULL;
	}
}

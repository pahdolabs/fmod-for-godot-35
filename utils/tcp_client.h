#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include "core/io/stream_peer_tcp.h"
#include "scene/main/timer.h"

#include "core/class_db.h"
#include "core/ustring.h"

class FmodTCPClient : public StreamPeerTCP
{
	GDCLASS(FmodTCPClient, StreamPeerTCP);

protected:
	static void _bind_methods()
	{
		ClassDB::bind_method(D_METHOD("on_connection_timeout"), &FmodTCPClient::on_connection_timeout);
		ADD_SIGNAL(MethodInfo("connection_finished"));
	};

private:
	const String HOST = "127.0.0.1";
	const int PORT = 3663;
	Timer* timer;

public:
	bool client_is_connected = false;

	void connect_client(Node* node)
	{
		if (get_status() != Status::STATUS_NONE)
		{
			return;
		}

		if (!node)
		{
			return;
		}

		if (connect_to_host(HOST, PORT) == Error::OK)
		{
			timer = memnew(Timer);
			timer->connect("timeout", this, "on_connection_timeout");
			timer->set_wait_time(0.3);
			timer->set_autostart(true);

			node->add_child(timer);
			timer->set_owner(node);
		}
	}

	void disconnect_client()
	{
		disconnect_from_host();
	}

	void on_connection_timeout()
	{
		if (get_status() == Status::STATUS_CONNECTED)
		{
			client_is_connected = true;
		}

		if (client_is_connected)
		{
			String out_result;
			Error result = get_command("", out_result);
			if (result == Error::FAILED)
			{
				disconnect_from_host();
				client_is_connected = false;
			}
		}
		else
		{
			disconnect_from_host();
		}

		emit_signal("connection_finished");

		if (timer)
		{
			if (!timer->is_queued_for_deletion())
			{
				timer->queue_delete();
			}
		}
	}

	Error get_command(const String& command, String& result)
	{
		CharString charstr = command.utf8();
		Vector<uint8_t> command_val;
		size_t len = charstr.length();
		command_val.resize(len);
		memcpy(command_val.ptrw(), charstr.ptr(), len);
		
		if (put_data(command_val.ptrw(), len) != Error::OK)
		{
			return Error::FAILED;
		}

		int32_t bytes = get_available_bytes();

		PoolByteArray bytes_array;
		if (bytes >= 0)
		{
			bytes_array.resize(bytes);
			
			if (get_data(bytes_array.write().ptr(), bytes) != Error::OK)
			{
				return Error::FAILED;
			}
		}
		else
		{
			return Error::FAILED;
		}
		int err = bytes_array[0];
		if (err != Error::OK)
		{
			return Error::FAILED;
		}

		PoolByteArray data_array = bytes_array;

		if (data_array.empty())
		{
			while (data_array.empty())
			{
				int bytes = get_available_bytes();
				bytes_array.resize(bytes);
				get_data(bytes_array.write().ptr(), bytes);
				data_array = bytes_array;
			}
		}

		while (data_array[data_array.size() - 1] != 0)
		{
			int bytes = get_available_bytes();
			bytes_array.resize(bytes);
			get_data(bytes_array.write().ptr(), bytes);
			data_array = bytes_array;
		}

		if (data_array.size() == 0)
		{
			return Error::FAILED;
		}

		if (data_array[data_array.size() - 1] == 0)
		{
			CharString message_char;
			message_char.resize(data_array.size());
			memcpy(message_char.ptrw(), data_array.read().ptr(), data_array.size());
			
			String message = String(message_char);
			message = message.replace("out(): ", "");
			result = message;
			return Error::OK;
		}
		else
		{
			return Error::FAILED;
		}
	}
};
#endif // TCP_CLIENT_H
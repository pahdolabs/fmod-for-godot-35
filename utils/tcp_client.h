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
			timer->set_one_shot(true);

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
		if (get_status() == Status::STATUS_CONNECTING) {
			timer->start();
			return;
		}
		
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
		Vector<uint8_t> utf8_buffer;
		int len = charstr.length();
		utf8_buffer.resize(len);
		memcpy(utf8_buffer.ptrw(), charstr.ptr(), len);
		
		if (put_data(utf8_buffer.ptr(), len) != Error::OK)
		{
			return Error::FAILED;
		}

		int32_t bytes = get_available_bytes();
		utf8_buffer.clear();
		utf8_buffer.resize(bytes);

		if (bytes >= 0)
		{
			int error = get_data(utf8_buffer.ptrw(), bytes);
			if (error != Error::OK)
			{
				return Error::FAILED;
			}
		}
		else
		{
			return Error::FAILED;
		}
		
		if (utf8_buffer.empty())
		{
			while (utf8_buffer.empty())
			{
				bytes = get_available_bytes();
				utf8_buffer.resize(bytes);
				get_data(utf8_buffer.ptrw(), bytes);
			}
		}

		while (utf8_buffer[utf8_buffer.size() - 1] != 0)
		{
			bytes = get_available_bytes();
			utf8_buffer.resize(bytes);
			get_data(utf8_buffer.ptrw(), bytes);
		}

		if (utf8_buffer.empty())
		{
			return Error::FAILED;
		}

		StreamPeerBuffer string_buffer;
		string_buffer.put_data(utf8_buffer.ptr(), bytes);
		if (utf8_buffer[utf8_buffer.size() - 1] == 0)
		{
			String message = string_buffer.get_utf8_string(bytes).strip_escapes();
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
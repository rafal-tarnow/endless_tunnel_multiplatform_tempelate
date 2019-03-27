package com.reyfel.sample.CapAfri;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

public class EchoServer implements Runnable {

    private DatagramSocket socket;
    private InetAddress address;
    private boolean running;
    private byte[] buf = new byte[256];

    public EchoServer() {

        try {
            socket = new DatagramSocket();
        } catch (SocketException e) {
            e.printStackTrace();
        }
        try {
            address = InetAddress.getByName("127.0.0.1");
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
    }

    public void run()
    {
        running = true;

        while (running)
        {

            try {
                sendEcho("123456");
            } catch (IOException e) {
                e.printStackTrace();
            }

//            if (received.equals("end")) {
//                running = false;
//                continue;
//            }

            try {
                Thread.sleep(250);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

        }
        //socket.close();


    }

    public boolean sendEcho(String msg) throws IOException {
        buf = msg.getBytes();
        DatagramPacket packet = new DatagramPacket(buf, buf.length, address, 7001);
        socket.send(packet);
        return true;
    }

    public void close() {
        socket.close();
    }
}

package com.reyfel.sample.CapAfri;

import android.net.LocalSocket;
import android.net.LocalSocketAddress;
import static android.net.LocalSocket.SOCKET_DGRAM;

import java.io.IOException;
import java.io.OutputStream;



public class LocalSocketWrapper implements Runnable {

    final String LOCAL_ADDR = "to_NDK";
    private LocalSocket mLocalSocket;
    private boolean running;
    private byte[] buf = new byte[256];

    public LocalSocketWrapper() {
        mLocalSocket = new LocalSocket(SOCKET_DGRAM);
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

            try {
                Thread.sleep(250);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

        }

        try {
            mLocalSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    public boolean sendEcho(String msg) throws IOException {
        buf = msg.getBytes();

        LocalSocketAddress.Namespace namespace;

        namespace = LocalSocketAddress.Namespace.ABSTRACT;
        LocalSocketAddress address = new LocalSocketAddress(LOCAL_ADDR, namespace);

        mLocalSocket.connect(address);

        OutputStream outputStream = mLocalSocket.getOutputStream();
        outputStream.write(buf);
        outputStream.flush();
        outputStream.close();

        try {
            mLocalSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return true;
    }

    public void close()
    {
        try {
            mLocalSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

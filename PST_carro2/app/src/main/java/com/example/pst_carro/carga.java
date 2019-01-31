package com.example.pst_carro;

import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.JsonReader;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;


public class carga extends AppCompatActivity {

    ArrayAdapter mArrayAdapter;
    ListView listView;
    TextView textViewPeso, textViewLista;
    Button buttonReiniciarLista, buttonCargar;
    double pesoLimite = 120;
    double pesoCarga = 0;
    String id_usuario;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_carga);

        listView = (ListView) findViewById(R.id.listView);
        textViewPeso = (TextView) findViewById(R.id.textViewPeso);
        textViewLista = (TextView) findViewById(R.id.textViewLista);
        buttonCargar = (Button) findViewById(R.id.buttonCargar);
        buttonCargar.setEnabled(true);
        buttonReiniciarLista = (Button) findViewById(R.id.buttonReiniciarLista);
        mArrayAdapter = new ArrayAdapter(this,R.layout.textviewlayout);
        listView.setAdapter(mArrayAdapter);

        Bundle bundle = getIntent().getExtras();
        id_usuario = bundle.getString("id_usuario");

        new consultarItems().execute("http://172.20.141.9/consultar_item.php?id_usuario="+id_usuario);

        buttonCargar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent i = new Intent(carga.this,bluetooth.class);
                startActivity(i);
            }
        });

        buttonReiniciarLista.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(carga.this,"Lista Reiniciada",Toast.LENGTH_SHORT).show();
            }
        });

    }


    private class consultarItems extends AsyncTask<String, Void, String> {
        @Override
        protected String doInBackground(String... urls) {

            // params comes from the execute() call: params[0] is the url.
            try {
                return downloadUrl(urls[0]);
            } catch (IOException e) {
                return "Unable to retrieve web page. URL may be invalid.";
            }
        }
        // onPostExecute displays the results of the AsyncTask.
        @Override
        protected void onPostExecute(String result) {

            JSONArray ja = null;
            String[] token;
            String[] token2;
            int length = result.length();
            int i;
            token=result.split("]");
            try {
                for (i=0;i<length;i++){
                    token2=token[i].replace("[","").replace("\"","").split(",");
                    ja = new JSONArray(token2);
                    mArrayAdapter.add(ja.getString(0) + "\n" + ja.getString(1));
                }
            } catch (JSONException e) {
                e.printStackTrace();
            }

        }
    }

    private String downloadUrl(String myurl) throws IOException {
        Log.i("URL",""+myurl);
        myurl = myurl.replace(" ","%20");
        InputStream is = null;
        // Only display the first 500 characters of the retrieved
        // web page content.
        int len = 500;

        try {
            URL url = new URL(myurl);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setReadTimeout(10000 /* milliseconds */);
            conn.setConnectTimeout(15000 /* milliseconds */);
            conn.setRequestMethod("GET");
            conn.setDoInput(true);
            // Starts the query
            conn.connect();
            int response = conn.getResponseCode();
            Log.d("respuesta", "The response is: " + response);
            is = conn.getInputStream();

            // Convert the InputStream into a string
            String contentAsString = readIt(is, len);
            return contentAsString;

            // Makes sure that the InputStream is closed after the app is
            // finished using it.
        } finally {
            if (is != null) {
                is.close();
            }
        }
    }

    public String readIt(InputStream stream, int len) throws IOException, UnsupportedEncodingException {
        Reader reader = null;
        reader = new InputStreamReader(stream, "UTF-8");
        char[] buffer = new char[len];
        reader.read(buffer);
        return new String(buffer);
    }
}

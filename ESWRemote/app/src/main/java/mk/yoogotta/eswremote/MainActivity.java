package mk.yoogotta.eswremote;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.TextHttpResponseHandler;

import cz.msebera.android.httpclient.Header;

public class MainActivity extends AppCompatActivity {

    TextView buzzerStatus;
    Button buzzer;
    Button buzzerSwitch;

    String serverUrl = "http://10.2.8.12:9999/";

    AsyncHttpClient client;

    Boolean isEnabled = false;
    String status[] = { "DISABLED", "ENABLED" };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        buzzerStatus = findViewById(R.id.textview);
        buzzer = findViewById(R.id.button);
        buzzerSwitch = findViewById(R.id.buzzer_switch);
        client = new AsyncHttpClient();

        client.post(serverUrl + "status/", new TextHttpResponseHandler() {
            @Override
            public void onFailure(int statusCode, Header[] headers, String responseString, Throwable throwable) {
                buzzerStatus.setText("Status UNKNOWN");
            }

            @Override
            public void onSuccess(int statusCode, Header[] headers, String responseString) {
                int index = responseString.equals("True") ? 1 : 0;
                isEnabled = index == 1;
                buzzerStatus.setText("BUZZER " + status[1 - index]);
                buzzerSwitch.setText(status[index]);
            }
        });

        buzzer.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                client.post(serverUrl + "BUZZ/", new TextHttpResponseHandler() {
                    @Override
                    public void onFailure(int i, Header[] headers, String s, Throwable throwable) {
                        Toast.makeText(getApplicationContext(),
                                "Some error occurred while communicating with the server",
                                Toast.LENGTH_LONG).show();
                    }

                    @Override
                    public void onSuccess(int i, Header[] headers, String s) {
                        Toast.makeText(getApplicationContext(), "Next will buzz!", Toast.LENGTH_LONG).show();
                    }
                });
            }});

        buzzerSwitch.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                client.post(serverUrl + "status/", new TextHttpResponseHandler() {
                    @Override
                    public void onFailure(int statusCode, Header[] headers, String responseString, Throwable throwable) {
                        Toast.makeText(getApplicationContext(), "Unable to get status.", Toast.LENGTH_LONG).show();
                    }

                    @Override
                    public void onSuccess(int statusCode, Header[] headers, String responseString) {
                        isEnabled = responseString.equals("True");
                    }
                });

                client.post(serverUrl + (isEnabled ? "enable/" : "disable/"), new TextHttpResponseHandler() {
                    @Override
                    public void onFailure(int statusCode, Header[] headers, String responseString, Throwable throwable) {
                        Toast.makeText(getApplicationContext(), "Status not changed :(", Toast.LENGTH_LONG).show();

                        int index = isEnabled ? 1 : 0;
                        buzzerStatus.setText("BUZZER " + status[1 - index]);
                        buzzerSwitch.setText(status[index]);
                    }

                    @Override
                    public void onSuccess(int statusCode, Header[] headers, String responseString) {
                        Toast.makeText(getApplicationContext(), "Status succesfully changed!", Toast.LENGTH_LONG).show();
                    }
                });
            }
        });
    }
}

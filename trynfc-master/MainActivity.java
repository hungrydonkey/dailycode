package com.frost;


import android.app.PendingIntent;
import android.content.Intent;
import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.os.Parcelable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private  NfcAdapter mNfcAdapter;
    private PendingIntent mPendingIntent;
    private Tag mTag;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

    }

    @Override
    protected  void onStart(){
        super.onStart();
        mNfcAdapter = NfcAdapter.getDefaultAdapter(this );
        if(mNfcAdapter == null ){
            Toast.makeText(this,"not support NFC",Toast.LENGTH_SHORT).show();
            finish();
            return;
        }
        if(!mNfcAdapter.isEnabled()){
            Toast.makeText(this,"please open  NFC",Toast.LENGTH_SHORT).show();
            finish();
            return;
        }
        Toast.makeText(this,"NFC is ready",Toast.LENGTH_SHORT).show();

        mPendingIntent = PendingIntent.getActivity(this,0,new Intent(this ,getClass()).addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP),0);
    }

    @Override
    protected void onNewIntent(Intent intent){
        super.onNewIntent(intent);
        mTag = intent.getParcelableExtra(NfcAdapter.EXTRA_TAG);
        Toast.makeText(this,"find a card",Toast.LENGTH_SHORT).show();
        String[]  techList = mTag.getTechList();
        Toast.makeText(this,"support types:",Toast.LENGTH_SHORT).show();
        for(String tech:techList){
            Toast.makeText(this,tech,Toast.LENGTH_SHORT).show();
        }
    }
    @Override
    protected  void onResume(){
        super.onResume();
        if(mNfcAdapter != null){
            mNfcAdapter.enableForegroundDispatch(this  ,mPendingIntent,null,null);
        }
    }
    @Override
    protected  void onPause(){
        super.onPause();
        if(mNfcAdapter != null){
            mNfcAdapter.disableForegroundDispatch(this);
        }
    }
}
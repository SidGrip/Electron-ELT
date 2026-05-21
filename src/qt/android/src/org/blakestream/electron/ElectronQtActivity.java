package org.blakestream.electron;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class ElectronQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File electronDir = new File(getFilesDir().getAbsolutePath() + "/.electron");
        if (!electronDir.exists()) {
            electronDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}

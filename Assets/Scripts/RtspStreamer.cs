using System.Collections;
using System.Collections.Generic;
using UnityEngine;
// IntPtr型を使用するのに必要
using System;
// Dllの読み込みに必要
using System.Runtime.InteropServices;
using System.IO;

public class RtspStreamer : MonoBehaviour
{

    [DllImport("librtsp-streamer-clib.so")] public static extern void Initialize(int format, string url, int width, int height, int image_size);
    [DllImport("librtsp-streamer-clib.so")] public static extern void SetImage(IntPtr pImage, int size);
    [DllImport("librtsp-streamer-clib.so")] public static extern void Finalize();

    // public Camera camera;
    public RenderTexture _renderTexture;
    public float _fps = 0.5f;
    private Texture2D _texture;
    private Texture2D _flipped_texture;
    private Rect _rect;
    private int _dataSize;
    private float _timeElapsed = 0.0f;
    private int _width;
    private int _height;


    void Start()
    {
        Debug.Log("Enter : Start()");

        _width = _renderTexture.width;
        _height = _renderTexture.height;
        int depth = 4;
        _dataSize = _width * _height * depth;

        _texture = new Texture2D(_width, _height, TextureFormat.BGRA32, false);
        _flipped_texture = new Texture2D(_width, _height, TextureFormat.BGRA32, false);
        _rect = new Rect(0, 0, _width, _height);

        Debug.Log("Width : " + _width.ToString());
        Debug.Log("Height : " + _height.ToString());
        Debug.Log("Depth : " + depth.ToString());
        Debug.Log("Size : " + _dataSize.ToString());

        Initialize(2, "/test", _width, _height, depth);

        Debug.Log("Exit : Start()");
    }

    void Update()
    {
        Debug.Log("Enter : Update()");
        _timeElapsed += Time.deltaTime;
        if (_timeElapsed >= _fps) {
            try {
                var currentRenderTexture = RenderTexture.active;
                RenderTexture.active = _renderTexture;
                _texture.ReadPixels(_rect, 0, 0);
                _texture.Apply();
                RenderTexture.active = currentRenderTexture;
                FlipTexture(_texture);
                SetImage(GCHandle.Alloc(_texture.GetPixels32(0), GCHandleType.Pinned).AddrOfPinnedObject(), _dataSize);

                Debug.Log("SetImage()");
            } catch (Exception e) {
                Debug.Log(e.Message);
            }
            _timeElapsed = 0.0f;
        }
        Debug.Log("Exit : Update()");
    }

    void FlipTexture(Texture2D tex)
    {
        var pixels = tex.GetPixels();

        Color[] newPixels = new Color[pixels.Length];

        for (int x = 0; x < _width; x++)
        {
            for (int y = 0; y < _height; y++)
            {
                newPixels[x + y * _width] = pixels[x + (_height - y -1) * _width];
            }
        }

        tex.SetPixels(newPixels);
        tex.Apply();
    }
}

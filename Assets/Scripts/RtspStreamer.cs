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
    private Rect _rect;
    private int _dataSize;
    private float _timeElapsed = 0.0f;

    void Start()
    {
        Debug.Log("Enter : Start()");

        int width = _renderTexture.width;
        int height = _renderTexture.height;
        int depth = 4;
        _dataSize = width * height * depth;

        _texture = new Texture2D(width, height, TextureFormat.BGRA32, false);
        _rect = new Rect(0, 0, width, height);

        Debug.Log("Width : " + width.ToString());
        Debug.Log("Height : " + height.ToString());
        Debug.Log("Depth : " + depth.ToString());
        Debug.Log("Size : " + _dataSize.ToString());

        Initialize(2, "/test", width, height, depth);

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
                SetImage(GCHandle.Alloc(_texture.GetPixels32(0), GCHandleType.Pinned).AddrOfPinnedObject(), _dataSize);

                Debug.Log("SetImage()");
            } catch (Exception e) {
                Debug.Log(e.Message);
            }
            _timeElapsed = 0.0f;
        }
        Debug.Log("Exit : Update()");
    }
}

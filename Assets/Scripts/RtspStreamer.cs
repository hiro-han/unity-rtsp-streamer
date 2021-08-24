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
    private Texture2D _texture;
    private Rect _rect;
    private int _dataSize;
    private float _timeElapsed = 0.0f;
    private float _timeOut = 2.0f;
    private bool _isFirst = true;

    // Start is called before the first frame update
    void Start()
    {
        Debug.Log("Enter : Start()");

        int width = _renderTexture.width;
        int height = _renderTexture.height;
        int depth = 4;
        _dataSize = width * height * depth;

        // camera = GameObject.Find("RtspCamera").GetComponent<Camera> ();
        _texture = new Texture2D(width, height, TextureFormat.BGRA32, false);
        _rect = new Rect(0, 0, width, height);

        Debug.Log("Width : " + width.ToString());
        Debug.Log("Height : " + height.ToString());
        Debug.Log("Depth : " + depth.ToString());
        Debug.Log("Size : " + _dataSize.ToString());

        Initialize(1, "/test", width, height, depth);

        Debug.Log("Exit : Start()");
    }

    // Update is called once per frame
    void Update()
    {
        Debug.Log("Enter : Update()");
        _timeElapsed += Time.deltaTime;
        // if (!_isFirst) {
        //     return;
        // }
        if (_timeElapsed >= _timeOut) {
            _isFirst = false;
            try {
                // RenderTexture.active = camera.targetTexture;
                // texture.ReadPixels(rect, 0, 0);
                // SetImage(texture.GetNativeTexturePtr(), data_size);
                // SetImage(camera.targetTexture.GetNativeTexturePtr(), data_size);

                // var tmp_rt = RenderTexture.active;
                // RenderTexture.active = _renderTexture;
                // // 取り込んで
                // _texture.ReadPixels(_rect, 0, 0);
                // _texture.Apply();
                // 戻す


                // RenderTexture.active = null;
                // RenderTexture.active = tmp_rt;


                // var png = _texture.EncodeToPNG();
                // File.WriteAllBytes("text.png", png );

                // var pixels = _texture.GetPixels();
                // GCHandle pixelHandle = GCHandle.Alloc(pixels, GCHandleType.Pinned);
                // IntPtr pixelPointer = pixelHandle.AddrOfPinnedObject();


                // SetImage(pixelPointer, _dataSize);
                // SetImage(_texture.GetRawTextureData(), _dataSize);
                // SetImage(_texture.GetNativeTexturePtr(), _dataSize);

                var currentRenderTexture = RenderTexture.active;
                RenderTexture.active = _renderTexture;
                _texture = new Texture2D(_renderTexture.width, _renderTexture.height, TextureFormat.BGRA32, false);
                _texture.ReadPixels(new Rect(0, 0, _renderTexture.width, _renderTexture.height), 0, 0);
                _texture.Apply();
                RenderTexture.active = currentRenderTexture;
                Debug.Log("ptr");
                Debug.Log(_texture.GetNativeTexturePtr());
                SetImage(GCHandle.Alloc(_texture.GetPixels32(0), GCHandleType.Pinned).AddrOfPinnedObject(), _dataSize);
                // SetImage(_texture.GetNativeTexturePtr(), _dataSize);


                // ここから
                // SetImage(_renderTexture.GetNativeTexturePtr(), _dataSize/2);
                // ここまで
                Debug.Log("SetImage()");
            } catch (Exception e) {
                Debug.Log(e.Message);
            }
            _timeElapsed = 0.0f;
        }
        Debug.Log("Exit : Update()");
    }
}

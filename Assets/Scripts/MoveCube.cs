using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveCube : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        transform.position = new Vector3(0f, 0f, 0f);
    }

    // Update is called once per frame
    void Update()
    {
        float angle = Time.time * 20 * Mathf.PI / 180;

        float x = Mathf.Cos(angle);
        float y = Mathf.Sin(angle);

        transform.position = new Vector3(x, y, 0f);
    }
}

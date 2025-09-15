using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MiObjeto : MonoBehaviour
{
	public string txtimagen;

	public string txtnombre;

	public string txtgenero;

	public GameObject objfoto;

	public TextMesh objnombre;

	public GameObject objgenero;

	public List<Texture2D> genero;

	private void Start()
	{
		StartCoroutine(getImage(txtimagen));
		objnombre.text = txtnombre;
		objgenero.get_renderer().material.mainTexture = genero[2];
		if (txtgenero == "m")
		{
			objgenero.get_renderer().material.mainTexture = genero[0];
		}
		if (txtgenero == "f")
		{
			objgenero.get_renderer().material.mainTexture = genero[1];
		}
	}

	public IEnumerator getImage(string imageURL)
	{
		WWW www = new WWW(imageURL);
		yield return www;
		Texture2D foto = www.texture;
		objfoto.get_renderer().material.mainTexture = foto;
	}
}

using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Xml;
using UnityEngine;

[ExecuteInEditMode]
[AddComponentMenu("TextFx/EffectManager")]
[RequireComponent(typeof(MeshFilter))]
[RequireComponent(typeof(MeshRenderer))]
public class EffectManager : MonoBehaviour
{
	public delegate void OnAnimationFinish();

	private const float FONT_SCALE_FACTOR = 10f;

	private const float BASE_LINE_HEIGHT = 1.05f;

	public static string m_version = "v2.1";

	public Font m_font;

	public TextAsset m_font_data_file;

	public Material m_font_material;

	public string m_text = string.Empty;

	public Vector2 m_px_offset = new Vector2(0f, 0f);

	public float m_character_size = 1f;

	public TextDisplayAxis m_display_axis;

	public TextAnchor m_text_anchor = TextAnchor.MiddleCenter;

	public TextAlignment m_text_alignment;

	public float m_line_height = 1f;

	public AnimatePerOptions m_animate_per;

	public AnimationTime m_time_type;

	public float m_begin_delay;

	public float m_max_width;

	public int m_number_of_words = -1;

	public int m_number_of_lines = -1;

	public List<TextSizeData> m_text_datas;

	public bool m_begin_on_start;

	public ON_FINISH_ACTION m_on_finish_action;

	private CombineInstance[] m_mesh_combine_instance;

	public LetterAnimation m_master_animation;

	public List<LetterAnimation> m_master_animations;

	public LetterSetup[] m_letters;

	private Transform m_transform_reference;

	private Renderer m_renderer;

	private MeshFilter m_mesh_filter;

	private float m_total_text_width;

	private float m_total_text_height;

	private List<AudioSource> m_audio_sources;

	private List<ParticleEmitter> m_particle_emitters;

	private List<ParticleEffectInstanceManager> m_effect_managers;

	private float m_last_time;

	private CustomFontCharacterData m_custom_font_data;

	private string m_current_font_data_file_name = string.Empty;

	private float m_animation_timer;

	private int m_lowest_action_progress;

	private bool m_running;

	private bool m_paused;

	private int m_editor_action_idx;

	private float m_editor_action_progress;

	private OnAnimationFinish m_animation_callback;

	private float FontScale => 10f / m_character_size;

	public Transform m_transform => m_transform_reference;

	private MeshFilter Mesh_Filter => m_mesh_filter;

	public int EditorActionIdx => m_editor_action_idx;

	public float EditorActionProgress => m_editor_action_progress;

	private float LineHeight => m_line_height * 1.05f;

	public bool IsFontDataAssigned
	{
		get
		{
			if (m_font != null)
			{
				return true;
			}
			if (m_font_data_file != null && m_font_material != null)
			{
				return true;
			}
			return false;
		}
	}

	public bool Playing => m_running && !m_paused;

	public bool Paused
	{
		get
		{
			return m_paused;
		}
		set
		{
			m_paused = value;
			if (!m_paused && m_time_type == AnimationTime.REAL_TIME)
			{
				m_last_time = Time.realtimeSinceStartup;
			}
		}
	}

	private void OnEnable()
	{
		m_mesh_filter = base.gameObject.GetComponent<MeshFilter>();
		m_transform_reference = base.transform;
		if (m_mesh_filter.sharedMesh != null)
		{
			EffectManager[] array = UnityEngine.Object.FindObjectsOfType(typeof(EffectManager)) as EffectManager[];
			EffectManager[] array2 = array;
			foreach (EffectManager effectManager in array2)
			{
				MeshFilter mesh_Filter = effectManager.Mesh_Filter;
				if (mesh_Filter != null && mesh_Filter.sharedMesh == m_mesh_filter.sharedMesh && mesh_Filter != m_mesh_filter)
				{
					m_mesh_filter.mesh = new Mesh();
					m_letters = new LetterSetup[0];
					SetText(m_text);
				}
			}
		}
		else
		{
			m_mesh_filter.mesh = new Mesh();
			if (IsFontDataAssigned)
			{
				SetText(m_text);
			}
		}
	}

	private void Start()
	{
		if (Application.isPlaying && m_begin_on_start)
		{
			PlayAnimation(m_begin_delay);
		}
	}

	private void HandleLegacyAnimationInstance()
	{
		if (m_master_animation != null && m_master_animation.m_letter_actions.Count > 0)
		{
			Debug.LogWarning("Converting Legacy TextFx animation.");
			m_master_animations = new List<LetterAnimation>();
			m_master_animations.Add(m_master_animation);
			m_master_animation = null;
			SetText(m_text);
		}
	}

	public void PlayAnimation(OnAnimationFinish animation_callback)
	{
		m_animation_callback = animation_callback;
		PlayAnimation(0f);
	}

	public void PlayAnimation(float delay, OnAnimationFinish animation_callback)
	{
		m_animation_callback = animation_callback;
		PlayAnimation(delay);
	}

	public void PlayAnimation(float delay = 0f)
	{
		HandleLegacyAnimationInstance();
		if (m_master_animations == null || m_master_animations.Count == 0)
		{
			Debug.LogWarning("Unable to execute PlayAnimation(). No animations defined on this EffectManager instance");
			return;
		}
		int num = m_letters.Length;
		m_audio_sources = new List<AudioSource>(base.gameObject.GetComponentsInChildren<AudioSource>());
		foreach (AudioSource audio_source in m_audio_sources)
		{
			audio_source.Stop();
		}
		m_particle_emitters = new List<ParticleEmitter>(base.gameObject.GetComponentsInChildren<ParticleEmitter>());
		m_effect_managers = new List<ParticleEffectInstanceManager>();
		foreach (ParticleEmitter particle_emitter in m_particle_emitters)
		{
			particle_emitter.emit = false;
			particle_emitter.particles = null;
			particle_emitter.enabled = false;
		}
		foreach (LetterAnimation master_animation in m_master_animations)
		{
			PrepareAnimationData(master_animation);
			master_animation.CurrentAnimationState = LETTER_ANIMATION_STATE.PLAYING;
			foreach (int item in master_animation.m_letters_to_animate)
			{
				if (item < num)
				{
					m_letters[item].Reset(master_animation);
					m_letters[item].Active = true;
				}
			}
		}
		m_mesh_combine_instance = new CombineInstance[m_letters.Length];
		m_lowest_action_progress = 0;
		m_animation_timer = 0f;
		if (delay > 0f)
		{
			StartCoroutine(PlayAnimationAfterDelay(delay));
			return;
		}
		if (Mesh_Filter.sharedMesh == null || Mesh_Filter.sharedMesh.vertexCount == 0)
		{
			SetText(m_text);
		}
		if (m_time_type == AnimationTime.REAL_TIME)
		{
			m_last_time = Time.realtimeSinceStartup;
		}
		m_running = true;
		m_paused = false;
	}

	private IEnumerator PlayAnimationAfterDelay(float delay)
	{
		if (Mesh_Filter.mesh == null || Mesh_Filter.mesh.vertexCount == 0)
		{
			SetText(m_text);
		}
		yield return StartCoroutine(TimeDelay(delay, m_time_type));
		if (m_time_type == AnimationTime.REAL_TIME)
		{
			m_last_time = Time.realtimeSinceStartup;
		}
		m_running = true;
		m_paused = false;
	}

	public void ResetAnimation()
	{
		SetAnimationState(0, 0f);
		m_running = false;
		m_paused = false;
		StopAllParticleEffects(force_stop: true);
	}

	public void SetEndState()
	{
		m_running = false;
		m_paused = false;
		int num = 0;
		foreach (LetterAnimation master_animation in m_master_animations)
		{
			if (master_animation.m_letter_actions.Count > num)
			{
				num = master_animation.m_letter_actions.Count;
			}
		}
		SetAnimationState(num - 1, 1f);
	}

	public void PrepareAnimationData(LetterAnimation animation)
	{
		animation.PrepareData(m_letters, m_number_of_words, m_number_of_lines, m_animate_per);
	}

	private void Update()
	{
		if (Application.isPlaying && m_running && !m_paused)
		{
			UpdateAnimation((m_time_type != 0) ? (Time.realtimeSinceStartup - m_last_time) : Time.deltaTime);
			if (m_time_type == AnimationTime.REAL_TIME)
			{
				m_last_time = Time.realtimeSinceStartup;
			}
		}
	}

	private void GetCharacterInfo(char m_character, out CustomCharacterInfo char_info)
	{
		char_info = null;
		if (m_font != null)
		{
			CharacterInfo info = default(CharacterInfo);
			m_font.GetCharacterInfo(m_character, out info);
			char_info = new CustomCharacterInfo();
			char_info.flipped = info.flipped;
			char_info.uv = info.uv;
			char_info.vert = info.vert;
			char_info.width = info.width;
			char_info.vert.x /= FontScale;
			char_info.vert.y /= FontScale;
			char_info.vert.width /= FontScale;
			char_info.vert.height /= FontScale;
			char_info.width /= FontScale;
			if (info.width == 0f)
			{
				Debug.LogWarning("Character '" + m_character + "' not found. Check that font '" + m_font.name + "' is NOT set to be Dynamic, and that the font supports this character. Try setting font to Unicode character set in font Import Settings.");
			}
		}
		if (m_font_data_file != null && char_info == null)
		{
			char_info = new CustomCharacterInfo();
			if (m_custom_font_data == null || !m_font_data_file.name.Equals(m_current_font_data_file_name))
			{
				if (m_font_data_file.text.Substring(0, 5).Equals("<?xml"))
				{
					m_current_font_data_file_name = m_font_data_file.name;
					m_custom_font_data = new CustomFontCharacterData();
					XmlTextReader xmlTextReader = new XmlTextReader(new StringReader(m_font_data_file.text));
					int num = 0;
					int num2 = 0;
					while (xmlTextReader.Read())
					{
						if (xmlTextReader.IsStartElement())
						{
							if (xmlTextReader.Name.Equals("common"))
							{
								num = int.Parse(xmlTextReader.GetAttribute("scaleW"));
								num2 = int.Parse(xmlTextReader.GetAttribute("scaleH"));
							}
							else if (xmlTextReader.Name.Equals("char"))
							{
								int num3 = int.Parse(xmlTextReader.GetAttribute("x"));
								int num4 = int.Parse(xmlTextReader.GetAttribute("y"));
								float num5 = float.Parse(xmlTextReader.GetAttribute("width"));
								float num6 = float.Parse(xmlTextReader.GetAttribute("height"));
								float x = float.Parse(xmlTextReader.GetAttribute("xoffset"));
								float num7 = float.Parse(xmlTextReader.GetAttribute("yoffset"));
								float width = float.Parse(xmlTextReader.GetAttribute("xadvance"));
								CustomCharacterInfo customCharacterInfo = new CustomCharacterInfo();
								customCharacterInfo.flipped = false;
								customCharacterInfo.uv = new Rect((float)num3 / (float)num, 1f - (float)num4 / (float)num2 - num6 / (float)num2, num5 / (float)num, num6 / (float)num2);
								customCharacterInfo.vert = new Rect(x, 0f - num7, num5, 0f - num6);
								customCharacterInfo.width = width;
								m_custom_font_data.m_character_infos.Add(int.Parse(xmlTextReader.GetAttribute("id")), customCharacterInfo);
							}
						}
					}
				}
				else if (m_font_data_file.text.Substring(0, 4).Equals("info"))
				{
					m_current_font_data_file_name = m_font_data_file.name;
					m_custom_font_data = new CustomFontCharacterData();
					int num8 = 0;
					int num9 = 0;
					string[] array = m_font_data_file.text.Split('\n');
					string[] array2 = array;
					foreach (string text in array2)
					{
						if (text.Length >= 5 && text.Substring(0, 5).Equals("char "))
						{
							string[] array3 = ParseFieldData(text, new string[8]
							{
								"id=",
								"x=",
								"y=",
								"width=",
								"height=",
								"xoffset=",
								"yoffset=",
								"xadvance="
							});
							int num10 = int.Parse(array3[1]);
							int num11 = int.Parse(array3[2]);
							float num12 = float.Parse(array3[3]);
							float num13 = float.Parse(array3[4]);
							float x2 = float.Parse(array3[5]);
							float num14 = float.Parse(array3[6]);
							float width2 = float.Parse(array3[7]);
							CustomCharacterInfo customCharacterInfo2 = new CustomCharacterInfo();
							customCharacterInfo2.flipped = false;
							customCharacterInfo2.uv = new Rect((float)num10 / (float)num8, 1f - (float)num11 / (float)num9 - num13 / (float)num9, num12 / (float)num8, num13 / (float)num9);
							customCharacterInfo2.vert = new Rect(x2, 0f - num14 + 1f, num12, 0f - num13);
							customCharacterInfo2.width = width2;
							m_custom_font_data.m_character_infos.Add(int.Parse(array3[0]), customCharacterInfo2);
						}
						else if (text.Length >= 6 && text.Substring(0, 6).Equals("common"))
						{
							string[] array3 = ParseFieldData(text, new string[3]
							{
								"scaleW=",
								"scaleH=",
								"lineHeight="
							});
							num8 = int.Parse(array3[0]);
							num9 = int.Parse(array3[1]);
						}
					}
				}
			}
			if (m_custom_font_data.m_character_infos.ContainsKey((int)m_character))
			{
				char_info = ((CustomCharacterInfo)m_custom_font_data.m_character_infos[(int)m_character]).ScaleClone(FontScale);
			}
		}
		else if (char_info == null)
		{
			char_info = new CustomCharacterInfo();
		}
	}

	private string[] ParseFieldData(string data_string, string[] fields)
	{
		string[] array = new string[fields.Length];
		int num = 0;
		foreach (string text in fields)
		{
			int num2 = data_string.IndexOf(text) + text.Length;
			int num3 = data_string.IndexOf(" ", num2);
			array[num] = data_string.Substring(num2, num3 - num2);
			num++;
		}
		return array;
	}

	public void SetText(string new_text)
	{
		//IL_0295: Unknown result type (might be due to invalid IL or missing references)
		//IL_029c: Expected O, but got Unknown
		if (m_renderer == null)
		{
			m_renderer = base.get_renderer();
		}
		bool flag = false;
		if ((m_renderer.sharedMaterial == null || m_renderer.sharedMaterial != m_font_material) && m_font_material != null)
		{
			m_renderer.sharedMaterial = m_font_material;
		}
		else if (m_font != null)
		{
			if (m_renderer.sharedMaterial == null || m_renderer.sharedMaterial != m_font_material)
			{
				m_font_material = m_font.material;
				m_renderer.sharedMaterial = m_font_material;
			}
			if (m_renderer.sharedMaterial != null)
			{
				flag = true;
			}
		}
		if (!flag && (m_renderer.sharedMaterial == null || m_font_data_file == null))
		{
			Debug.LogWarning("SetText() : Incomplete font setup information. Check that you've assigned your font files in the inspector.");
			return;
		}
		m_text = new_text;
		new_text = new_text.Replace("\r", string.Empty);
		string text = m_text.Replace(" ", string.Empty);
		text = text.Replace("\n", string.Empty);
		text = text.Replace("\r", string.Empty);
		text = text.Replace("\t", string.Empty);
		int length = text.Length;
		int length2 = new_text.Length;
		LetterSetup[] letters = m_letters;
		m_letters = new LetterSetup[length];
		CustomCharacterInfo last_char_info = null;
		m_text_datas = new List<TextSizeData>();
		float y_max = 0f;
		float y_min = 0f;
		float x_max = 0f;
		float x_min = 0f;
		float text_width = 0f;
		float text_height = 0f;
		int line_letter_idx = 0;
		float line_height_offset = 0f;
		float total_text_width = 0f;
		float total_text_height = 0f;
		float line_width_at_last_space = 0f;
		float space_char_offset = 0f;
		int last_letter_setup_idx = -1;
		float last_space_y_max = 0f;
		float last_space_y_min = 0f;
		float num = 0f;
		int num2 = 0;
		int num3 = 0;
		int num4 = 0;
		Action val = (Action)(object)(Action)delegate
		{
			if (m_display_axis == TextDisplayAxis.HORIZONTAL)
			{
				float num11 = Mathf.Abs(y_max - y_min) * LineHeight;
				if (last_char_info != null)
				{
					text_width += 0f - last_char_info.width + last_char_info.vert.width + last_char_info.vert.x;
				}
				m_text_datas.Add(new TextSizeData(text_width, num11, line_height_offset, y_max));
				line_height_offset += num11;
				if (text_width > total_text_width)
				{
					total_text_width = text_width;
				}
				total_text_height += num11;
			}
			else
			{
				float num12 = Mathf.Abs(x_max - x_min) * LineHeight;
				m_text_datas.Add(new TextSizeData(num12, text_height * -1f, line_height_offset, 0f));
				line_height_offset += num12;
				total_text_width += num12;
				if (text_height < total_text_height)
				{
					total_text_height = text_height;
				}
			}
			line_letter_idx = 0;
			text_width = 0f;
			line_width_at_last_space = 0f;
			space_char_offset = 0f;
			last_space_y_max = 0f;
			last_space_y_min = 0f;
			last_letter_setup_idx = -1;
			text_height = 0f;
			last_char_info = null;
		};
		for (int i = 0; i < length2; i++)
		{
			char c = new_text[i];
			GetCharacterInfo(c, out CustomCharacterInfo char_info);
			if (char_info != null)
			{
				if (c.Equals('\t'))
				{
					continue;
				}
				if (c.Equals(' '))
				{
					if (m_display_axis == TextDisplayAxis.HORIZONTAL)
					{
						line_width_at_last_space = text_width;
						space_char_offset = char_info.width;
						last_space_y_max = y_max;
						last_space_y_min = y_min;
						last_letter_setup_idx = num2;
						text_width += char_info.width;
					}
					else
					{
						char_info.vert.height = 0f - char_info.width;
					}
					num += ((m_display_axis != 0) ? (0f - char_info.width) : char_info.width);
					num4++;
				}
				else if (c.Equals('\n'))
				{
					val.Invoke();
					num = 0f;
					num3++;
					num4++;
				}
				else
				{
					if (m_display_axis == TextDisplayAxis.HORIZONTAL)
					{
						if (line_letter_idx == 0 || char_info.vert.y > y_max)
						{
							y_max = char_info.vert.y;
						}
						if (line_letter_idx == 0 || char_info.vert.y + char_info.vert.height < y_min)
						{
							y_min = char_info.vert.y + char_info.vert.height;
						}
						text_width += ((i != length2 - 1) ? char_info.width : (char_info.vert.width + char_info.vert.x));
						if (m_max_width > 0f && last_letter_setup_idx >= 0)
						{
							float num5 = (i != length2 - 1) ? (text_width - char_info.width + char_info.vert.width + char_info.vert.x) : text_width;
							if (num5 > m_max_width)
							{
								float num6 = text_width - line_width_at_last_space - space_char_offset;
								float num7 = last_space_y_min;
								float num8 = last_space_y_max;
								text_width = line_width_at_last_space;
								y_max = last_space_y_max;
								y_min = last_space_y_min;
								num = 0f;
								num3++;
								for (int j = last_letter_setup_idx; j < num2; j++)
								{
									m_letters[j].m_progression_variables.m_line_value = num3;
									m_letters[j].m_base_offset = ((m_display_axis != 0) ? new Vector3(0f, num, 0f) : new Vector3(num, 0f, 0f));
									num += ((m_display_axis != 0) ? (m_letters[j].m_height + (0f - m_px_offset.y) / FontScale) : (m_letters[j].m_offset_width + m_px_offset.x / FontScale));
								}
								val.Invoke();
								text_width = num6;
								y_min = num7;
								y_max = num8;
							}
						}
					}
					else
					{
						if (line_letter_idx == 0 || char_info.vert.x + char_info.vert.width > x_max)
						{
							x_max = char_info.vert.x + char_info.vert.width;
						}
						if (line_letter_idx == 0 || char_info.vert.x < x_min)
						{
							x_min = char_info.vert.x;
						}
						text_height += char_info.vert.height;
					}
					if (letters != null && num2 < letters.Length && letters[num2].m_character.Equals(new_text[i].ToString()) && letters[num2].m_progression_variables.m_letter_value == i && letters[num2].m_mesh != null)
					{
						m_letters[num2] = letters[num2];
						letters[num2] = null;
						Vector3 base_offset = (m_display_axis != 0) ? new Vector3(0f, num, 0f) : new Vector3(num, 0f, 0f);
						m_letters[num2].m_base_offset = base_offset;
						m_letters[num2].SetupLetterMesh(char_info);
						m_letters[num2].m_progression_variables.m_line_value = num3;
						m_letters[num2].m_progression_variables.m_word_value = num4;
						m_letters[num2].m_base_offsets_setup = false;
					}
					else
					{
						Mesh mesh = new Mesh();
						Rect uv = char_info.uv;
						mesh.vertices = new Vector3[4]
						{
							Vector3.zero,
							Vector3.zero,
							Vector3.zero,
							Vector3.zero
						};
						mesh.uv = new Vector2[4]
						{
							new Vector2(uv.x + uv.width, uv.y + uv.height),
							new Vector2(uv.x, uv.y + uv.height),
							new Vector2(uv.x, uv.y),
							new Vector2(uv.x + uv.width, uv.y)
						};
						mesh.triangles = new int[6]
						{
							2,
							1,
							0,
							3,
							2,
							0
						};
						m_letters[num2] = new LetterSetup(string.Empty + c, num2, mesh, (m_display_axis != 0) ? new Vector3(0f, num, 0f) : new Vector3(num, 0f, 0f), char_info, num3, num4);
					}
					num2++;
					num += ((m_display_axis != 0) ? (char_info.vert.height + (0f - m_px_offset.y) / FontScale) : (char_info.width + m_px_offset.x / FontScale));
					last_char_info = char_info;
				}
			}
			line_letter_idx++;
		}
		m_number_of_words = num4 + 1;
		m_number_of_lines = num3 + 1;
		if (m_display_axis == TextDisplayAxis.HORIZONTAL)
		{
			float num9 = Mathf.Abs(y_max - y_min);
			m_text_datas.Add(new TextSizeData(text_width, num9, line_height_offset, y_max));
			if (text_width > total_text_width)
			{
				total_text_width = text_width;
			}
			total_text_height += num9;
		}
		else
		{
			float num10 = Mathf.Abs(x_max - x_min);
			m_text_datas.Add(new TextSizeData(num10, text_height * -1f, line_height_offset, 0f));
			total_text_width += num10;
			if (text_height < total_text_height)
			{
				total_text_height = text_height;
			}
		}
		m_total_text_width = total_text_width;
		m_total_text_height = total_text_height;
		for (int k = 0; k < m_text_datas.Count; k++)
		{
			m_text_datas[k].m_total_text_height = total_text_height * (float)((m_display_axis == TextDisplayAxis.HORIZONTAL) ? 1 : (-1));
			if (m_max_width > 0f)
			{
				m_text_datas[k].m_total_text_width = m_max_width;
			}
			else
			{
				m_text_datas[k].m_total_text_width = total_text_width;
			}
		}
		if (letters != null)
		{
			LetterSetup[] array = letters;
			foreach (LetterSetup letterSetup in array)
			{
				if (letterSetup != null)
				{
					if (Application.isPlaying)
					{
						UnityEngine.Object.Destroy(letterSetup.m_mesh);
					}
					else
					{
						UnityEngine.Object.DestroyImmediate(letterSetup.m_mesh);
					}
				}
			}
		}
		if (Application.isPlaying)
		{
			SetAnimationState(0, 0f);
		}
		else
		{
			SetAnimationState(m_editor_action_idx, m_editor_action_progress);
		}
	}

	public bool UpdateAnimation(float delta_time)
	{
		m_animation_timer += delta_time;
		if (UpdateMesh(use_timer: true, 0, 0f, delta_time))
		{
			m_running = false;
			if (m_animation_callback != null)
			{
				m_animation_callback();
			}
			if (Application.isPlaying)
			{
				if (m_on_finish_action == ON_FINISH_ACTION.DESTROY_OBJECT)
				{
					UnityEngine.Object.Destroy(base.gameObject);
				}
				else if (m_on_finish_action == ON_FINISH_ACTION.DISABLE_OBJECT)
				{
					base.gameObject.SetActive(value: false);
				}
				else if (m_on_finish_action == ON_FINISH_ACTION.RESET_ANIMATION)
				{
					ResetAnimation();
				}
			}
			StopAllParticleEffects();
		}
		if (m_running && m_effect_managers.Count > 0)
		{
			for (int i = 0; i < m_effect_managers.Count; i++)
			{
				ParticleEffectInstanceManager particleEffectInstanceManager = m_effect_managers[i];
				if (particleEffectInstanceManager.Update(delta_time))
				{
					m_effect_managers.RemoveAt(i);
					i--;
				}
			}
		}
		return m_running;
	}

	public void ContinueAnimation()
	{
		ContinueAnimation(-1);
	}

	public void ContinueAnimation(int animation_index)
	{
		if (animation_index >= 0)
		{
			ContinueAnimationState(animation_index);
			return;
		}
		for (int i = 0; i < m_master_animations.Count; i++)
		{
			ContinueAnimationState(i);
		}
	}

	private void ContinueAnimationState(int animation_index)
	{
		LetterAnimation letterAnimation = m_master_animations[animation_index];
		letterAnimation.CurrentAnimationState = LETTER_ANIMATION_STATE.PLAYING;
		foreach (int item in letterAnimation.m_letters_to_animate)
		{
			if (m_letters[item].WaitingToSync)
			{
				m_letters[item].ContinueAction(m_animation_timer, letterAnimation, m_animate_per);
			}
			else if (m_letters[item].ActiveLoopCycles.Count > 0)
			{
				m_letters[item].ActiveLoopCycles[0].m_number_of_loops = 1;
			}
		}
	}

	public void SetAnimationState(int action_idx, float action_progress)
	{
		if (!Application.isPlaying)
		{
			m_editor_action_idx = action_idx;
			m_editor_action_progress = action_progress;
		}
		HandleLegacyAnimationInstance();
		m_animation_timer = 0f;
		m_lowest_action_progress = -1;
		bool flag = true;
		do
		{
			flag = true;
			LetterSetup[] letters = m_letters;
			foreach (LetterSetup letterSetup in letters)
			{
				if (!letterSetup.m_base_offsets_setup)
				{
					if (m_text_datas.Count == 0)
					{
						flag = false;
						break;
					}
					letterSetup.SetBaseOffset(m_text_anchor, m_display_axis, m_text_alignment, m_text_datas);
				}
			}
			if (!flag)
			{
				Debug.LogError("If text_datas has been lost or if legacy effect and hasn't been created, reset text.");
				SetText(m_text);
			}
		}
		while (!flag);
		int num = m_letters.Length;
		if (m_master_animations != null)
		{
			foreach (LetterAnimation master_animation in m_master_animations)
			{
				PrepareAnimationData(master_animation);
				master_animation.CurrentAnimationState = LETTER_ANIMATION_STATE.PLAYING;
				foreach (int item in master_animation.m_letters_to_animate)
				{
					if (item < num)
					{
						m_letters[item].Reset(master_animation);
					}
				}
			}
		}
		UpdateMesh(use_timer: false, action_idx, action_progress, 0f);
	}

	private bool UpdateMesh(bool use_timer, int action_idx, float action_progress, float delta_time = 0f)
	{
		bool result = true;
		int num = -1;
		if (m_mesh_combine_instance == null || m_letters.Length != m_mesh_combine_instance.Length)
		{
			m_mesh_combine_instance = new CombineInstance[m_letters.Length];
		}
		int num2 = action_idx;
		bool[] array = new bool[m_letters.Length];
		if (m_master_animations != null)
		{
			foreach (LetterAnimation master_animation in m_master_animations)
			{
				num2 = Mathf.Clamp(action_idx, 0, master_animation.m_letter_actions.Count - 1);
				int num3 = -1;
				bool flag = true;
				foreach (int item in master_animation.m_letters_to_animate)
				{
					if (item == num3 || item >= m_letters.Length)
					{
						continue;
					}
					LetterSetup letterSetup = m_letters[item];
					if (num == -1 || letterSetup.ActionProgress < num)
					{
						num = letterSetup.ActionProgress;
					}
					if (use_timer)
					{
						LETTER_ANIMATION_STATE lETTER_ANIMATION_STATE = letterSetup.AnimateMesh(m_animation_timer, m_text_anchor, m_lowest_action_progress, master_animation, m_animate_per, delta_time, this);
						if (lETTER_ANIMATION_STATE == LETTER_ANIMATION_STATE.PLAYING || lETTER_ANIMATION_STATE == LETTER_ANIMATION_STATE.WAITING)
						{
							result = false;
						}
						if (lETTER_ANIMATION_STATE == LETTER_ANIMATION_STATE.PLAYING || lETTER_ANIMATION_STATE == LETTER_ANIMATION_STATE.STOPPED)
						{
							flag = false;
						}
					}
					else
					{
						letterSetup.SetMeshState(num2, action_progress, master_animation, m_animate_per);
					}
					m_mesh_combine_instance[item].mesh = letterSetup.m_mesh;
					array[item] = true;
					num3 = item;
				}
				master_animation.CurrentAnimationState = ((flag && use_timer) ? LETTER_ANIMATION_STATE.WAITING : LETTER_ANIMATION_STATE.PLAYING);
				if (num > m_lowest_action_progress)
				{
					m_lowest_action_progress = num;
				}
			}
		}
		for (int i = 0; i < array.Length; i++)
		{
			if (!array[i])
			{
				LetterSetup letterSetup = m_letters[i];
				letterSetup.SetMeshState(-1, 0f, null, AnimatePerOptions.LETTER);
				m_mesh_combine_instance[i].mesh = letterSetup.m_mesh;
			}
		}
		if (Mesh_Filter.sharedMesh == null)
		{
			Mesh_Filter.sharedMesh = new Mesh();
		}
		Mesh_Filter.sharedMesh.CombineMeshes(m_mesh_combine_instance, mergeSubMeshes: true, useMatrices: false);
		return result;
	}

	private void OnDestroy()
	{
		if (m_letters != null)
		{
			LetterSetup[] letters = m_letters;
			foreach (LetterSetup letterSetup in letters)
			{
				if (Application.isPlaying)
				{
					UnityEngine.Object.Destroy(letterSetup.m_mesh);
				}
				else
				{
					UnityEngine.Object.DestroyImmediate(letterSetup.m_mesh);
				}
			}
		}
		if (Application.isPlaying)
		{
			UnityEngine.Object.Destroy(Mesh_Filter.sharedMesh);
		}
		else
		{
			UnityEngine.Object.DestroyImmediate(Mesh_Filter.sharedMesh);
		}
	}

	private void OnDrawGizmos()
	{
		if (m_max_width > 0f)
		{
			Gizmos.color = Color.red;
			Vector3 zero = Vector3.zero;
			if (m_text_anchor == TextAnchor.LowerLeft || m_text_anchor == TextAnchor.MiddleLeft || m_text_anchor == TextAnchor.UpperLeft)
			{
				zero += new Vector3(((!(m_max_width > 0f)) ? m_total_text_width : m_max_width) / 2f, 0f, 0f);
			}
			else if (m_text_anchor == TextAnchor.LowerRight || m_text_anchor == TextAnchor.MiddleRight || m_text_anchor == TextAnchor.UpperRight)
			{
				zero -= new Vector3(((!(m_max_width > 0f)) ? m_total_text_width : m_max_width) / 2f, 0f, 0f);
			}
			if (m_text_anchor == TextAnchor.LowerCenter || m_text_anchor == TextAnchor.LowerLeft || m_text_anchor == TextAnchor.LowerRight)
			{
				zero += new Vector3(0f, m_total_text_height / 2f, 0f);
			}
			else if (m_text_anchor == TextAnchor.UpperLeft || m_text_anchor == TextAnchor.UpperCenter || m_text_anchor == TextAnchor.UpperRight)
			{
				zero -= new Vector3(0f, m_total_text_height / 2f, 0f);
			}
			if (m_max_width > 0f)
			{
				Gizmos.DrawWireCube(base.transform.position + zero - new Vector3(m_max_width / 2f, 0f, 0f), new Vector3(0.01f, m_total_text_height, 0f));
				Gizmos.DrawWireCube(base.transform.position + zero + new Vector3(m_max_width / 2f, 0f, 0f), new Vector3(0.01f, m_total_text_height, 0f));
			}
		}
	}

	private AudioSource AddNewAudioChild()
	{
		GameObject gameObject = new GameObject("TextFx_AudioSource");
		gameObject.transform.parent = base.transform;
		AudioSource audioSource = gameObject.AddComponent<AudioSource>();
		audioSource.playOnAwake = false;
		if (m_audio_sources == null)
		{
			m_audio_sources = new List<AudioSource>();
		}
		m_audio_sources.Add(audioSource);
		return audioSource;
	}

	private void PlayClip(AudioSource a_source, AudioClip clip, float delay, float start_time, float volume, float pitch)
	{
		a_source.clip = clip;
		a_source.time = start_time;
		a_source.volume = volume;
		a_source.pitch = pitch;
		a_source.Play((ulong)(delay * 44100f));
	}

	public void PlayAudioClip(AudioClip clip, float delay, float start_time, float volume, float pitch)
	{
		bool flag = false;
		if (m_audio_sources != null)
		{
			foreach (AudioSource audio_source in m_audio_sources)
			{
				if (!audio_source.isPlaying)
				{
					PlayClip(audio_source, clip, delay, start_time, volume, pitch);
					flag = true;
					break;
				}
			}
			if (!flag)
			{
				PlayClip(AddNewAudioChild(), clip, delay, start_time, volume, pitch);
			}
		}
		else
		{
			PlayClip(AddNewAudioChild(), clip, delay, start_time, volume, pitch);
		}
	}

	public void PlayParticleEffect(ParticleEmitter emitter_prefab, float delay, float duration, Mesh character_mesh, Vector3 position_offset, bool follow_mesh = false)
	{
		bool flag = false;
		if (m_particle_emitters == null)
		{
			return;
		}
		foreach (ParticleEmitter particle_emitter in m_particle_emitters)
		{
			if (!particle_emitter.emit && particle_emitter.particleCount == 0 && particle_emitter.name.Equals(emitter_prefab.name + "(Clone)"))
			{
				m_effect_managers.Add(new ParticleEffectInstanceManager(particle_emitter, this, character_mesh, delay, duration, position_offset, follow_mesh));
				flag = true;
				break;
			}
		}
		if (!flag)
		{
			ParticleEmitter particleEmitter = UnityEngine.Object.Instantiate((UnityEngine.Object)emitter_prefab) as ParticleEmitter;
			m_particle_emitters.Add(particleEmitter);
			particleEmitter.transform.parent = base.transform;
			m_effect_managers.Add(new ParticleEffectInstanceManager(particleEmitter, this, character_mesh, delay, duration, position_offset, follow_mesh));
		}
	}

	private void StopAllParticleEffects(bool force_stop = false)
	{
		if (m_effect_managers == null)
		{
			return;
		}
		foreach (ParticleEffectInstanceManager effect_manager in m_effect_managers)
		{
			effect_manager.Stop(force_stop);
		}
		m_effect_managers = new List<ParticleEffectInstanceManager>();
	}

	private IEnumerator TimeDelay(float delay, AnimationTime time_type)
	{
		if (time_type == AnimationTime.GAME_TIME)
		{
			yield return new WaitForSeconds(delay);
			yield break;
		}
		float timer = 0f;
		float last_time = Time.realtimeSinceStartup;
		while (timer < delay)
		{
			float delta_time = Time.realtimeSinceStartup - last_time;
			if (delta_time > 0.1f)
			{
				delta_time = 0.1f;
			}
			timer += delta_time;
			last_time = Time.realtimeSinceStartup;
			yield return false;
		}
	}

	public static Vector3 Vector3Lerp(Vector3 from_vec, Vector3 to_vec, float progress)
	{
		if (progress <= 1f && progress >= 0f)
		{
			return Vector3.Lerp(from_vec, to_vec, progress);
		}
		return from_vec + Vector3.Scale(to_vec - from_vec, Vector3.one * progress);
	}

	public static float FloatLerp(float from_val, float to_val, float progress)
	{
		if (progress <= 1f && progress >= 0f)
		{
			return Mathf.Lerp(from_val, to_val, progress);
		}
		return from_val + (to_val - from_val) * progress;
	}
}

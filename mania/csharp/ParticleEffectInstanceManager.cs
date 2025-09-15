using UnityEngine;

public class ParticleEffectInstanceManager
{
	private ParticleEmitter m_emitter;

	private EffectManager m_effect_manager_handle;

	private float m_duration;

	private float m_delay;

	private Mesh m_letter_mesh;

	private Vector3 m_position_offset;

	private bool m_follow_mesh;

	private bool m_active;

	private Transform m_transform;

	public bool Active => m_delay > 0f || m_duration > 0f || m_emitter.particleCount > 0;

	public ParticleEffectInstanceManager(ParticleEmitter p_emitter, EffectManager effect_manager, Mesh character_mesh, float delay, float duration, Vector3 position_offset, bool follow_mesh)
	{
		m_emitter = p_emitter;
		m_duration = duration;
		m_letter_mesh = character_mesh;
		m_delay = delay;
		m_position_offset = position_offset;
		m_follow_mesh = follow_mesh;
		m_effect_manager_handle = effect_manager;
		m_transform = m_emitter.transform;
		m_emitter.emit = true;
		m_emitter.enabled = false;
		m_letter_mesh.RecalculateNormals();
		if (!m_letter_mesh.normals[0].Equals(Vector3.zero))
		{
			Quaternion rotation = Quaternion.LookRotation(m_letter_mesh.normals[0], (!m_letter_mesh.vertices[1].Equals(m_letter_mesh.vertices[2])) ? (m_letter_mesh.vertices[1] - m_letter_mesh.vertices[2]) : Vector3.forward);
			m_transform.position = m_effect_manager_handle.m_transform.position + rotation * m_position_offset + (m_letter_mesh.vertices[0] + m_letter_mesh.vertices[1] + m_letter_mesh.vertices[2] + m_letter_mesh.vertices[3]) / 4f;
			m_transform.rotation = rotation;
		}
		else
		{
			m_transform.position = m_effect_manager_handle.m_transform.position + m_position_offset + (m_letter_mesh.vertices[0] + m_letter_mesh.vertices[1] + m_letter_mesh.vertices[2] + m_letter_mesh.vertices[3]) / 4f;
		}
	}

	public bool Update(float delta_time)
	{
		if (!m_active)
		{
			if (m_delay > 0f)
			{
				m_delay -= delta_time;
				if (m_delay < 0f)
				{
					m_delay = 0f;
				}
				return false;
			}
			m_active = true;
			m_emitter.emit = false;
			m_emitter.enabled = true;
			if (m_duration > 0f)
			{
				m_emitter.emit = true;
			}
			else
			{
				m_emitter.Emit();
			}
		}
		if (m_follow_mesh)
		{
			m_letter_mesh.RecalculateNormals();
			if (!m_letter_mesh.normals[0].Equals(Vector3.zero))
			{
				Quaternion rotation = Quaternion.LookRotation(m_letter_mesh.normals[0], (!m_letter_mesh.vertices[1].Equals(m_letter_mesh.vertices[2])) ? (m_letter_mesh.vertices[1] - m_letter_mesh.vertices[2]) : Vector3.forward);
				m_transform.position = m_effect_manager_handle.m_transform.position + rotation * m_position_offset + (m_letter_mesh.vertices[0] + m_letter_mesh.vertices[1] + m_letter_mesh.vertices[2] + m_letter_mesh.vertices[3]) / 4f;
				m_transform.rotation = rotation;
			}
			else
			{
				m_transform.position = m_effect_manager_handle.m_transform.position + m_position_offset + (m_letter_mesh.vertices[0] + m_letter_mesh.vertices[1] + m_letter_mesh.vertices[2] + m_letter_mesh.vertices[3]) / 4f;
			}
		}
		m_duration -= delta_time;
		if (m_duration > 0f)
		{
			return false;
		}
		m_emitter.emit = false;
		if (m_emitter.particleCount > 0)
		{
			return false;
		}
		return true;
	}

	public void Stop(bool force_stop)
	{
		m_emitter.emit = false;
		if (force_stop)
		{
			m_emitter.ClearParticles();
		}
	}
}

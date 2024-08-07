import { twMerge } from 'tailwind-merge';

export function NodeIcon({ txt }: { txt: string; className?: string }) {
  if (txt === 'switch') {
    return (
      <svg
        viewBox="0 0 36 36"
        version="1.1"
        preserveAspectRatio="xMidYMid meet"
        xmlns="http://www.w3.org/2000/svg"
        xmlnsXlink="http://www.w3.org/1999/xlink"
        className={twMerge('fill-base-content h-5 w-5')}
      >
        <title>network-switch-line</title>
        <path
          d="M33.91,18.47,30.78,8.41A2,2,0,0,0,28.87,7H7.13A2,2,0,0,0,5.22,8.41L2.09,18.48a2,2,0,0,0-.09.59V27a2,2,0,0,0,2,2H32a2,2,0,0,0,2-2V19.06A2,2,0,0,0,33.91,18.47ZM32,27H4V19.06L7.13,9H28.87L32,19.06Z"
          className="clr-i-outline clr-i-outline-path-1"
        ></path>
        <rect x="7.12" y="22" width="1.8" height="3" className="clr-i-outline clr-i-outline-path-2"></rect>
        <rect x="12.12" y="22" width="1.8" height="3" className="clr-i-outline clr-i-outline-path-3"></rect>
        <rect x="17.11" y="22" width="1.8" height="3" className="clr-i-outline clr-i-outline-path-4"></rect>
        <rect x="22.1" y="22" width="1.8" height="3" className="clr-i-outline clr-i-outline-path-5"></rect>
        <rect x="27.1" y="22" width="1.8" height="3" className="clr-i-outline clr-i-outline-path-6"></rect>
        <rect x="6.23" y="18" width="23.69" height="1.4" className="clr-i-outline clr-i-outline-path-7"></rect>
        <rect x="0" y="0" width="36" height="36" fillOpacity="0" />
      </svg>
    );
  } else if (txt === 'wap') {
    return (
      <svg
        version="1.1"
        xmlns="http://www.w3.org/2000/svg"
        viewBox="0 0 512 512"
        xmlnsXlink="http://www.w3.org/1999/xlink"
        enableBackground="new 0 0 512 512"
        className={twMerge('fill-base-content h-5 w-5')}
      >
        <g>
          <g>
            <path d="m449.3,297.7h-172.9v-82.3c0-11.3-9.1-20.4-20.4-20.4s-20.4,9.1-20.4,20.4v82.3h-172.9c-28.5,0-51.7,23.2-51.7,51.7v99.9c0,28.5 23.2,51.7 51.7,51.7h386.7c28.5,0 51.7-23.2 51.7-51.7v-99.9c-0.1-28.5-23.3-51.7-51.8-51.7zm10.9,151.6c0,6-4.9,10.8-10.8,10.8h-386.7c-6,0-10.8-4.9-10.8-10.8v-99.9c0-6 4.9-10.8 10.8-10.8h386.7c6,0 10.8,4.9 10.8,10.8v99.9z" />
            <path d="m156.2,93.1c55-55 144.5-55 199.6,0 4,4 9.2,6 14.4,6 5.2,0 10.5-2 14.4-6 8-8 8-20.9 0-28.9-70.9-70.9-186.3-70.9-257.3,0-8,8-8,20.9 0,28.9 8,8 20.9,8 28.9,0z" />
            <path d="m194.3,131.1c-8,8-8,20.9 0,28.9 4,4 9.2,6 14.4,6 5.2,0 10.4-2 14.4-6 18.1-18.1 47.6-18.1 65.7,0 8,8 20.9,8 28.9,0 8-8 8-20.9 0-28.9-34-34-89.4-34-123.4,0z" />
            <path d="m269.8,378.9h-27.6c-11.3,0-20.4,9.1-20.4,20.4s9.1,20.4 20.4,20.4h27.6c11.3,0 20.4-9.1 20.4-20.4s-9.1-20.4-20.4-20.4z" />
          </g>
        </g>
      </svg>
    );
  } else if (txt === 'edge_router') {
    return (
      <svg
        version="1.1"
        id="Layer_1"
        xmlns="http://www.w3.org/2000/svg"
        xmlnsXlink="http://www.w3.org/1999/xlink"
        viewBox="0 0 491.52 491.52"
        xmlSpace="preserve"
        className={twMerge('fill-base-content h-5 w-5')}
      >
        <g>
          <g>
            <path
              d="M491.52,450.56V327.68H327.68v51.2h-40.96v-87.71h68.01c55.53,0,95.83-36.32,95.83-86.37c0-36.38-23.23-68.81-56.66-81.11
			c3.05-10.11,4.14-20.6,3.41-32.27C394.24,41.41,349.72,0.4,298.07,0c-41.06-0.04-78.09,24.21-93.45,60.2
			c-15.1-11.2-34.18-16.6-53.59-14.79c-27.94,2.62-52.38,21.23-63.77,48.56c-5.42,12.99-4.7,27.42-3.46,36.22
			c-26.26,15.37-42.84,43.84-42.84,74.61c0,49.24,41.2,86.37,95.83,86.37h68.01v87.71h-40.96v-51.2H0v122.88h71.68v20.48H30.72
			v20.48h102.4v-20.48H92.16v-20.48h71.68v-51.2h61.44V291.17h40.96v108.19h61.44v51.2h71.68v20.48H358.4v20.48h102.4v-20.48h-40.96
			v-20.48H491.52z M143.36,430.08H20.48v-81.92h122.88V430.08z M136.79,270.69c-43.66,0-75.35-27.71-75.35-65.89
			c0-25.55,14.97-49.01,38.15-59.78l7.77-3.65l-2.2-8.3c-0.05-0.17-4.61-17.76,1-31.22c8.47-20.32,26.4-34.13,46.79-36.04
			c1.84-0.18,3.67-0.26,5.49-0.26c16.69,0,32.51,7.13,43.12,19.67l13.27,15.69l4.54-20.04c7.94-35.08,40.65-60.39,77.92-60.39
			c0.2,0,0.41,0,0.62,0c41.11,0.32,76.53,32.71,78.96,72.21c0.79,12.63-0.99,22.97-5.74,33.51l-5.21,11.53l12.36,2.69
			c30.02,6.53,51.8,33.61,51.8,64.38c0,38.8-30.99,65.89-75.35,65.89H136.79z M348.16,430.08v-81.92h122.88v81.92H348.16z"
            />
          </g>
        </g>
      </svg>
    );
  }
}
